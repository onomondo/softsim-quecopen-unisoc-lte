当前的hilinkSDK版本是HiLink+SDK+12.0.0.106_xrdebug (11) ：hilinksdk-v3-openadapter-12.0.1.106
2021/8/10下午

此版本打开宏
BUILD_MBEDTLS=1
SUPPORT_CJSON=1

printf做了映射
#ifdef CONFIG_PLATFORM_TL8251
#undef printf
#define printf(fmt, args...) HILINK_ql_printf(fmt, ##args)
#endif

编译链使用这个
set(CFG_HI_TOOLCHAINS_PATH "$ENV{TOOLCHAIN_PATH}/gcc-arm-none-eabi-10-2020-q4-major/bin")