

#include "hal_spi_flash.h"
#include "osi_compiler.h"
#include "hal_spi_flash.h"
#include "hal_chip.h"
#include "hwregs.h"
#include "osi_profile.h"
#include "osi_byte_buf.h"
#include "hal_spi_flash_defs.h"
#include <alloca.h>
#include "quec_proj_config.h"

/**
 * Flash property table. It is not necessary to put it in ram.
 */
halSpiFlash_t ql_SpiFlashProps[] = {
#include "hal_spi_flash_prop.h"
};
int ql_SpiFlashPropsLen = OSI_ARRAY_SIZE(ql_SpiFlashProps);

bool quec_spi_flash_props_init(halSpiFlash_t **props_ptr,int *props_len)
{
	if(props_ptr == NULL || props_len == NULL)
	{
		return false;
	}
	
	*props_ptr = ql_SpiFlashProps;
	*props_len = ql_SpiFlashPropsLen;
	return true;	
}

void quec_mipi_lcd_clk_set(void)
{
#ifndef CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD
	REG_CP_IDLE_PD_PLL_SW_T pd_pll_sw = {.b.dsipll = 1};
	hwp_idle->pd_pll_sw_set = pd_pll_sw.v;
	REG_CP_IDLE_PD_PLL_SEL_T pd_pll_sel = {.b.dsipll = 1};
	hwp_idle->pd_pll_sel_set = pd_pll_sel.v;
#else
	REG_CP_IDLE_PD_PLL_SW_T pd_pll_sw = {.b.dsipll = 0};
	hwp_idle->pd_pll_sw_set = pd_pll_sw.v;
	REG_CP_IDLE_PD_PLL_SEL_T pd_pll_sel = {.b.dsipll = 0};
	hwp_idle->pd_pll_sel_set = pd_pll_sel.v;
#endif
}

/* 23/05/2022
** 6 lines spi ext NorFlash mount sffs
** 0x80000 is designed to be compatible with the older version, which can be modified by new customers.
** The format takes effect only after the value is changed to 0.
*/
#define EXT_DEFAULT_RESERVED_SPACE_SIZE   0x80000     //0x80000是为了与老版本兼容,新客户可以自行修改。
//#define EXT_DEFAULT_RESERVED_SPACE_SIZE   0x0         //修改为0后,格式化后才会生效。
unsigned int quec_nor_ext_mount_start_addr = EXT_DEFAULT_RESERVED_SPACE_SIZE; //挂载的起始地址是CONFIG_NOR_EXT_PHY_ADDRESS(0x70000000)的偏移地址

/*
默认开启xip保护，若在外置flash存放代码，必须要开启。兼容旧程序配置。当外置flash的配置支持suspend时，擦写flash时，flash可能会进入suspend。
关闭xip保护时，擦写flash时，flash不会进入suspend，提高擦写效率
*/
bool quec_ext_flash_xip_protect = true;
/*
默认关闭外置flash mouth过程中的flash写保护功能。
当外置flash使用3.3V供电时，外置flash过程中断电，会导致文件内容丢失。开启后，由于flash写保护，不会改写flash，文件内容不会丢失
开启外置flash mouth过程中的flash写保护功能后，应用不能有调用ql_rename等更名操作，若在更名操作中断电，可能会导致下次挂载文件系统死机
*/
bool quec_ext_flash_mount_protect = false; 


