////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_2.0
// 본 소스코드의 소유권은 yss Embedded Operating System 네이버 카페 관리자와 운영진에게 있습니다.
// 운영진이 임의로 코드의 권한을 타인에게 양도할 수 없습니다.
// 본 소스코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스코드의 상업적 또는 비상업적 이용이 가능합니다.
// 본 소스코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스코드의 내용을 무단 전재하는 행위를 금합니다.
// 본 소스코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떤한 법적 책임을 지지 않습니다.
//
//	Home Page : http://cafe.naver.com/yssoperatingsystem
//	Copyright 2020.	yss Embedded Operating System all right reserved.
//
//  주담당자 : 아이구 (mymy49@nate.com) 2016.04.30 ~ 현재
//  부담당자 : -
//
////////////////////////////////////////////////////////////////////////////////////////

#include <__cross_studio_io.h>

#include <config.h>
#include <internal/malloc.h>

namespace Malloc
{
void *malloc(MallocSet &obj, unsigned long size)
{
	MallocTable *table;
	unsigned long mallocCluster, buffer;
	unsigned long cnt = 0, begin, shifter, index;
	unsigned long addr;
	unsigned long *cluster = obj.cluster;
	unsigned long needNumOfCluster = size / obj.clusterSize;
	bool checking = false, complete = false;

	if (size % obj.clusterSize)
		needNumOfCluster++;

	// 빈 테이블 검색
	for (signed long i = 0; i < obj.maxNumOfMalloc; i++)
	{
		if (!obj.table[i].addr)
		{
			table = &obj.table[i];
			goto next1;
		}
	}
	return 0;

next1:
	// 빈 클러스터 탐색
	for (unsigned long i = 0, index = 0xffffffff; i < obj.totalClusterNum * 32; i++)
	{
		if (i % 32 == 0)
		{
			index++;
			buffer = ~cluster[index];
			if (buffer == 0)
			{
				i += 31;
				goto next;
			}
			shifter = 1;
		}

		if (checking)
		{
			if (buffer & shifter)
			{
				cnt++;
			}
			else
			{
				checking = false;
				cnt = 0;
			}
		}
		else
		{
			if (buffer & shifter)
			{
				checking = true;
				begin = i;
				cnt++;
			}
		}

		if (needNumOfCluster == cnt)
		{
			complete = true;
			break;
		}
	next:
		shifter <<= 1;
	}

	if (complete == false)
		return 0;

	// 반환될 주소 계산
	addr = (unsigned long)obj.heap;
	addr += begin * obj.clusterSize;

	// 찾아진 주소 위치가 힙의 전체 크기를 초과하는지 검사
	if (addr + size > obj.endOfHeapAddr)
		return 0;

	// 할당된 클러스터 채우기
	shifter = 1 << (begin % 32);
	index = begin / 32;
	while (cnt)
	{
		if (shifter == 0)
		{
			shifter = 1;
			index++;
		}
		if (shifter == 1 && cnt > 32)
		{
			cluster[index] = 0xffffffff;
			shifter = 0;
			cnt -= 32;
		}
		else
		{
			cluster[index] |= shifter;
			shifter <<= 1;
			cnt--;
		}
	}

	// 할당 테이블에 데이터 저장
	table->addr = (void *)addr;
	table->begin = begin;
	table->clusterSize = needNumOfCluster;

	return (void *)addr;
}

void free(MallocSet &obj, void *addr)
{
	unsigned long shifter, index, cnt;
	MallocTable *table;

	// 할당된 클러스터 테이블 찾기
	for (unsigned long i = 0; i < obj.maxNumOfMalloc; i++)
	{
		if (obj.table[i].addr == addr)
		{
			table = &obj.table[i];
			goto next;
		}
	}
	return;

next:
	// 할당된 클러스터 지우기
	cnt = table->clusterSize;
	shifter = 1 << (table->begin % 32);
	index = table->begin / 32;
	while (cnt)
	{
		if (shifter == 0)
		{
			shifter = 1;
			index++;
		}
		if (shifter == 1 && cnt > 32)
		{
			obj.cluster[index] = 0x0;
			shifter = 0;
			cnt -= 32;
		}
		else
		{
			obj.cluster[index] &= ~shifter;
			shifter <<= 1;
			cnt--;
		}
	}

	table->addr = 0;
	table->begin = 0;
	table->clusterSize = 0;
}
}