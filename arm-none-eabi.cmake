add_compile_options(
  -std=gnu11
  -mcpu=cortex-a5
  -mtune=generic-armv7-a
  -mthumb
  -mfpu=neon-vfpv4
  -mfloat-abi=hard
  -mno-unaligned-access
  -g
  -Os
  -Wall
  -fno-strict-aliasing
  -ffunction-sections
  -fdata-sections)
