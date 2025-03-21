/**  @file
  quec_virt_func_common.c

  @brief
  This file is used to run virt function.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
22/06/2021        neo     Init version
=================================================================*/
#include "at_response.h"
#include "at_engine.h"
#include "quec_cust_feature.h"
#include "quec_common.h"
#include "at_command.h"
#include "at_param.h"
#include "ql_api_osi.h"
#include "quec_at_engine.h"
#include "quec_log.h"
#include "ql_lcd.h"
#include "ql_sdmmc.h"
#include "ql_audio.h"
#include "ql_gnss.h"
#include "ql_usb.h"
#include "ql_api_spi4_ext_nor_sffs.h"
#include "ql_api_spi6_ext_nor_flash.h"
#include "ql_api_spi_nand_flash.h"
#include "ql_wifi_scan.h"

#define QUEC_VIRT_FUNC_LOG(msg, ...)  custom_log("VIRT_FUNC", msg, ##__VA_ARGS__)

/*
__attribute__((weak)) const ql_at_desc_t *ql_atcmd_search_desc(const char *name, size_t length)
{
    QUEC_VIRT_FUNC_LOG("weak ql at search");
    return NULL;
}
*/

//LCD
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
__attribute__((weak)) int ql_compute_threshold(uint16_t *in_buf, int width, int height)
{
    return 0;
}

__attribute__((weak)) ql_errcode_lcd ql_spi_lcd_write_cmd(uint8_t cmd)
{
    return QL_LCD_EXECUTE_ERR;
}

__attribute__((weak)) ql_errcode_lcd ql_spi_lcd_write_data(uint8_t data)
{
    return QL_LCD_EXECUTE_ERR;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_LCD*/


#ifndef CONFIG_QUEC_PROJECT_FEATURE_WIFISCAN
__attribute__((weak)) ql_wifiscan_cfg_s ql_wifi_cfg = {.scan_timeout = false};
__attribute__((weak)) bool ql_wifiscan_get_open_state(void)
{
    return false;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BT
__attribute__((weak)) bool ql_bt_ble_is_using()
{
    return false;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SDMMC
__attribute__((weak)) void quec_exec_sdmmc_mkfs_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_sdmmc_mount_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_sdmmc_qft_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) bool ql_sdmmc_is_mount(void)
{
    return FALSE; 
}

__attribute__((weak)) bool ql_sdmmc_is_mount_ex(ql_sdmmc_partition_num_e part)
{
    return FALSE; 
}

__attribute__((weak)) ql_errcode_sdmmc_e ql_sdmmc_mkfs(uint8_t opt)
{
    return QL_SDMMC_MKFS_ERR;
}

__attribute__((weak)) ql_errcode_sdmmc_e ql_sdmmc_mount(void)
{
    return QL_SDMMC_MOUNT_ERR;
}

__attribute__((weak)) ql_errcode_sdmmc_e ql_sdmmc_umount(void)
{
    return QL_SDMMC_MOUNT_ERR;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
uint8_t ql_spi6_ext_nor_flash_clk = SPI6_EXT_NOR_FLASH_CLK_83MHZ;
__attribute__((weak)) bool ql_get_spi6_ext_nor_status(void)
{
    return false;
}

__attribute__((weak)) void quec_set_spi6_ext_nor_flash_capacity(void)
{
    return ;
}

__attribute__((weak)) int ql_spi6_ext_nor_flash_clk_set(uint8_t clk)
{
    return 0;
}

__attribute__((weak)) int quec_spi6_ext_nor_flash_mount(bool format_on_fail)
{
    return 0;
}

__attribute__((weak)) void quec_boot_spi6_ext_norflash_gpio_init(void)
{
    return ;
}

__attribute__((weak)) void quec_boot_get_spi6_ext_norflash_gpio_default_fuction(void)
{
    return ;
}

__attribute__((weak)) void quec_boot_spi6_ext_norflash_gpio_deinit(void)
{
    return ;
}
__attribute__((weak)) void quec_exec_efs_mkfs_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void ql_boot_set_skip_spi6_ext_norflash_init(bool onff)
{
    return ;
}
__attribute__((weak)) void quec_spi6_ext_norflash_set_mount_status(bool onff)
{
    return ;
}
__attribute__((weak)) bool quec_spi6_ext_norflash_get_mount_status(void)
{
    return false;
}
__attribute__((weak)) ql_errcode_spi6_nor_e ql_spi6_ext_nor_flash_sffs_mkfs(void)
{
    return QL_SPI6_EXT_NOR_FLASH_NOT_INIT_ERROR;
}

__attribute__((weak)) void quec_spi6_ext_nor_flash_mount_range(unsigned int *start, unsigned int *size)
{
    return ;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS

__attribute__((weak)) bool quec_spi4_exnsffs_is_mount(void)
{
    return false;
}

__attribute__((weak)) void quec_exec_exnsffs_mkfs_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) ql_spi4_extnsffs_status_e ql_spi4_ext_nor_sffs_is_mount(void)
{
    return (ql_spi4_extnsffs_status_e)0;
}

#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE_FS_NAND_FLASH
__attribute__((weak)) uint32_t ql_nand_total_erase_cnt = 0;
__attribute__((weak)) unsigned int  *ql_nand_erase_count_table = NULL;
__attribute__((weak)) unsigned int ql_nand_block_totalnum = 0;

__attribute__((weak)) ql_spi_port_e ql_nand_spi_port = QL_SPI_PORT1;
__attribute__((weak)) ql_nand_ops_t *nand_opt = NULL;

__attribute__((weak)) void quec_exec_nand_mkfs_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_nand_mount_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
__attribute__((weak)) bool ql_nand_flash_is_mount(void)
{
    return false;
}

#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI6_NAND_FLASH
__attribute__((weak)) void ql_spi6_nand_wakeup(void){}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_GNSS
nmeasrc_sentences nmea_handle = {0};
uint8_t quec_gnss_nwrtc_update_flag = 0;
quec_gps_handle quec_gps_config_param = {0};
__attribute__((weak)) ql_GnssState ql_gnss_state_info_get(void)
{
    return GNSS_CLOSE;
}

__attribute__((weak)) void quec_gps_config_param_read(void)
{
    return ;
}
#endif


#ifndef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
ql_headset_info_t ql_headset_info = {NULL, QL_HEADSET_CBTYPE_NONE};

__attribute__((weak)) void ql_audio_start_ring_handler(void)
{
	return;
}

__attribute__((weak)) void ql_audio_stop_ring_handler(void)
{
	return;
}

__attribute__((weak)) QL_AUD_RING_TYPE_E ql_aud_get_ringtone_type(void)
{
	return QL_AUD_RING_NONE;
}

__attribute__((weak)) ql_audio_errcode_e ql_aud_set_ringtone_type(QL_AUD_RING_TYPE_E type)
{
	return QL_AUDIO_OPER_NOT_SUPPORTED;
}

__attribute__((weak)) QL_AUD_SPKPA_TYPE_E ql_aud_get_pa_type(void)
{
	return QL_AUD_SPKPA_TYPE_CLASSAB;
}

__attribute__((weak)) bool ql_aud_nvm_init(void)
{
	return -1;
}

__attribute__((weak)) uint8 uinsoc_volume_to_ql_vol(uint8 volume, uint8 *res)
{
	return 0;
}
__attribute__((weak)) uint8 ql_volume_to_unisoc_vol(AUDIOHAL_SPK_LEVEL_T volume, uint8 *res)
{
	return 0;
}

__attribute__((weak)) ql_audio_errcode_e ql_set_volume(AUDIOHAL_SPK_LEVEL_T vol)
{
	return QL_AUDIO_OPER_NOT_SUPPORTED;
}
__attribute__((weak)) int ql_pcm_close(PCM_HANDLE_T handle)
{
    return QL_AUDIO_INVALID_PARAM;
}
__attribute__((weak)) PCM_HANDLE_T ql_aud_pcm_open(QL_PCM_CONFIG_T *config, AudStreamFormat_e format, unsigned int flags, pcm_path_e type)
{
    return NULL;
}
__attribute__((weak)) ql_audio_errcode_e ql_pcm_buffer_reset(PCM_HANDLE_T handle)
{
    return QL_AUDIO_INVALID_PARAM;
}
__attribute__((weak)) int ql_pcm_read(PCM_HANDLE_T handle, void *data, unsigned int count)
{
    return -1; 
}
__attribute__((weak)) int ql_pcm_write(PCM_HANDLE_T handle, void *data, unsigned int count)
{
    return -1;
}
__attribute__((weak)) void ql_audio_notify_handler(QL_AUDIO_NOTIFY_E type, void *param)
{
    return;
}
__attribute__((weak)) void ql_ext_pa_ctrl(AudOutputType_e path, uint8 on)
{
    return;
}
__attribute__((weak)) int ql_get_audio_state(void)
{
    return -1;
}
__attribute__((weak)) ql_audio_errcode_e ql_aud_recorder_pause(void)
{
    return QL_AUDIO_RECORD_SYS_FAIL;
}
__attribute__((weak)) bool ql_get_audio_poc_type(void)
{
    return false;
}
__attribute__((weak)) ql_audio_errcode_e ql_aud_recorder_resume(void)
{
    return QL_AUDIO_RECORD_SYS_FAIL;
}
__attribute__((weak)) ql_audio_errcode_e ql_aud_player_pause(void)
{
    return QL_AUDIO_RECORD_SYS_FAIL;
}
__attribute__((weak)) ql_audio_errcode_e ql_aud_player_resume(void)
{
    return QL_AUDIO_RECORD_SYS_FAIL;
}
__attribute__((weak)) void quec_aud_data_arrive(char *buf, uint size)
{
    return;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_DTMF
unsigned ql_dtmf_det_cfg[AUDIOHAL_DTMF_MAX]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};//灵敏度之前的默认值是2，保持不变

__attribute__((weak)) int quec_DtmfCharToTone(char c)
{
    return -1;
}

__attribute__((weak)) ql_audio_errcode_e ql_aud_tone_det_level_get(QL_TONE_DET_LEVEL_E *level)
{
    return QL_AUDIO_OPER_NOT_SUPPORTED;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_USB
__attribute__((weak)) ql_errcode_usb_e ql_set_usb_detect_max_time(uint32_t ms)
{
    return QL_USB_SYS_ERROR;
}

__attribute__((weak)) ql_errcode_usb_e ql_usb_set_detmode(ql_usb_detmdoe_t *usb_mode)
{
    return QL_USB_SYS_ERROR;
}

__attribute__((weak)) ql_errcode_usb_e ql_usb_get_detmode(ql_usb_detmdoe_t *mode)
{
    return QL_USB_SYS_ERROR;
}

__attribute__((weak)) ql_errcode_usb_e ql_usb_hotplug_init(void)
{
    return QL_USB_SYS_ERROR;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_DEC_AAC
int ql_aud_aac_open(const void *buf, uint size, void *dec_buf, ql_aud_config *config)
{
	return -1;
}

int ql_aud_aac_close(void)
{
	return -1;
}

int ql_aud_aac_write(const void *buf, uint size)
{
	return -1;
}

int ql_aud_aac_end(void)
{
	return -1;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_FIREWALL_AT
__attribute__((weak)) void at_firewall_qfirewallcfg_cmd_func(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#endif

#ifndef CONFIG_AT_WIFISCAN_SUPPORT
__attribute__((weak)) void atCmdHandleWifiOpen(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void atCmdHandleWifiClose(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void atCmdHandleWifiScan(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#ifndef CONFIG_AT_WIFISCAN_SUPPORT
__attribute__((weak)) void atCmdHandleWifiSensitivityTest(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#endif
#endif


#ifndef CONFIG_QUEC_PROJECT_FEATURE_FILE
__attribute__((weak)) QFILE ql_fopen_ex(const char *file_name, const char *mode, size_t size)
{
    return -1;
}
__attribute__((weak)) QFILE ql_open_ex(const char *file_name, int mode, bool open_unisoc, size_t size)
{
    return -1;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SLAS
__attribute__((weak)) void quec_exec_qlicense_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_qlicenseinfo_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_FATFS_SAFE
__attribute__((weak)) int logfs_mount(void* fs,uint32_t boot_sect,uint32_t ls_cust_cnt)
{
	return 1;
}
__attribute__((weak)) int logfs_umount(void* fs,uint32_t boot_sect)
{
	return 1;
}
__attribute__((weak)) int logfs_write_file(void* fs, void* fp, void* data, uint32_t data_len)
{
	return 1;
}
__attribute__((weak)) int logfs_write_filen_header(void* fs, void* fp, void* dir, char is_open)
{
	return 1;
}
__attribute__((weak)) int logfs_read_file(void* fs, void* fp)
{
	return 1;
}
__attribute__((weak)) int logfs_read_file0(void* fs)
{
	return 1;
}
__attribute__((weak)) char logfs_find_file_clst(void * fs, void* fp)
{
    return 0xff;
}
__attribute__((weak)) char logfs_set_file_clst(void * fs, void* fp)
{
    return 0;
}
__attribute__((weak)) void logfs_set_opt_prop(void* fs, void* prop)
{}
__attribute__((weak)) void* logfs_get_opt_prop(void* fs)
{
	return NULL;
}

#endif


#ifndef CONFIG_QUEC_PROJECT_FEATURE_VIRT_AT
__attribute__((weak)) int ql_virt_at_set_simid(ql_virt_at_port_number_e port, uint8 sim_id)
{
	return -1;	
}

__attribute__((weak)) int ql_virt_at_get_simid(ql_virt_at_port_number_e port)
{
	return -1;
}
#endif
