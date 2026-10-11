#-------------------------------------------------------------------------------
# STM32H5 target (Cortex-M33, TrustZone disabled: TZEN=0, single non-secure image)
#
# Drivers are written with STM32 LL. The application provides the vendor files
# through the yss_vendor INTERFACE target:
#   - CMSIS Core 6            (core_cm33.h)
#   - cmsis-device-h5         (stm32h5xx.h)
#   - stm32h5xx-hal-driver    (stm32h5xx_ll_*.h)
#
# system_stm32h5xx.c must NOT be linked: yss msp provides SystemInit() and
# SystemCoreClockUpdate().
#-------------------------------------------------------------------------------
file(GLOB_RECURSE YSS_TARGET_SRC CONFIGURE_DEPENDS
  ${YSS_DIR}/src/targets/st/*.cpp
  ${YSS_DIR}/src/targets/st/*.c
  ${YSS_DIR}/src/targets/common/*.cpp
  )
