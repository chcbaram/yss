#-------------------------------------------------------------------------------
# yss static library (CMake + arm-none-eabi-gcc)
#
# The original project is built with SEGGER Embedded Studio. This file adds a
# CMake build without changing the SEGGER build: SEGGER-specific behavior is
# handled with compile definitions here, not by editing sources.
#
# The application provides:
#   YSS_TARGET      target family, selects cmake/targets/<YSS_TARGET>.cmake (e.g. stm32h5)
#   YSS_MCU         MCU part-number macro (e.g. STM32H563xx)
#   YSS_CONFIG_DIR  directory containing the application's config.h
#   YSS_HEAP_SIZE   heap size in bytes, must match the linker script (__HEAP_SIZE__)
#   yss_vendor      INTERFACE target: vendor include dirs, CPU flags
#
# Usage:
#   add_subdirectory(lib/yss)
#   target_link_libraries(app.elf PRIVATE yss)
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

foreach(_var YSS_TARGET YSS_MCU YSS_CONFIG_DIR YSS_HEAP_SIZE)
  if(NOT DEFINED ${_var})
    message(FATAL_ERROR "yss: ${_var} is not set")
  endif()
endforeach()

if(NOT TARGET yss_vendor)
  message(FATAL_ERROR "yss: INTERFACE target 'yss_vendor' is not defined")
endif()

set(YSS_DIR ${CMAKE_CURRENT_SOURCE_DIR})


# Common sources. Vendor directories under src/targets are added by the target
# file only. Files for other families compile to nothing because of #if guards.
#
file(GLOB_RECURSE YSS_COMMON_SRC CONFIGURE_DEPENDS
  ${YSS_DIR}/src/*.cpp
  ${YSS_DIR}/src/*.c
  ${YSS_DIR}/src/*.s
  )
list(FILTER YSS_COMMON_SRC EXCLUDE REGEX "${YSS_DIR}/src/targets/[^/]+/")

set(YSS_TARGET_SRC "")
set(YSS_TARGET_FILE ${YSS_DIR}/cmake/targets/${YSS_TARGET}.cmake)
if(NOT EXISTS ${YSS_TARGET_FILE})
  message(FATAL_ERROR "yss: unknown YSS_TARGET '${YSS_TARGET}' (${YSS_TARGET_FILE})")
endif()
include(${YSS_TARGET_FILE})


add_library(yss STATIC
  ${YSS_COMMON_SRC}
  ${YSS_TARGET_SRC}
  )

target_include_directories(yss PUBLIC
  ${YSS_DIR}/inc
  ${YSS_CONFIG_DIR}
  )

target_compile_definitions(yss PUBLIC
  ${YSS_MCU}
  __HEAP_SIZE__=${YSS_HEAP_SIZE}  # SEGGER defines this automatically (yss_hmalloc.cpp)
  __SEGGER_LINKER                 # debug.h: declare debug_printf instead of including <__cross_studio_io.h>
  )

target_link_libraries(yss PUBLIC yss_vendor)

target_compile_options(yss PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti -fno-threadsafe-statics>
  -fdata-sections
  -ffunction-sections
  )

# yss .s files contain #include / #if, and GCC does not preprocess lower-case .s
#
set_source_files_properties(${YSS_COMMON_SRC} ${YSS_TARGET_SRC}
  TARGET_DIRECTORY yss
  PROPERTIES COMPILE_OPTIONS "$<$<COMPILE_LANGUAGE:ASM>:-x;assembler-with-cpp>"
  )
