
/**  @file
  ql_sdmmc_demo.c

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
                
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ql_log.h"
#include "osi_api.h"
#include "ql_sdmmc.h"
#include "ql_sdmmc_demo.h"
#include "ql_api_osi.h"
#include "ql_fs.h"
#include "ql_gpio.h"
#include "ql_pin_cfg.h"

/*===========================================================================
 *Definition
 ===========================================================================*/
#define QL_SDMMC_DEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_SDMMC_DEMO_LOG(msg, ...)         QL_LOG(QL_SDMMC_DEMO_LOG_LEVEL, "SDMMC_DEMO", msg, ##__VA_ARGS__)
#define QL_SDMMC_DEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_SDMMC_DEMO", msg, ##__VA_ARGS__)

#define QL_FM_FAT32                     0x02
#define QL_SDMMC_TASK_STACK_SIZE        4096
#define QL_SDMMC_TASK_PRIO              APP_PRIORITY_NORMAL
#define QL_SDMMC_TASK_EVENT_CNT         5

#define QL_SDMMC_FILE_PATH              "SD:test.txt"
#define QL_SDMMC_FILE_PATH1             "SD1:test.txt"
#define QL_SDMMC_TEST_STR               "1234567890abcdefg"
#define QL_SDMMC_CLK_FREQ               25000000
#define QL_SDMMC_BLOCK_NUM              10
#define QL_SDMMC_FS_TEST                0  //开启文件系统api测试
#define QL_SDMMC_MUTIL_PARTITION_TEST   0  //多分区功能

#define	QL_SDMMC_EVENT_PLUGOUT          0
#define	QL_SDMMC_EVENT_INSERT           1
#define QL_SDMMC_DET_TEST               1

/*#############################################################################*/

#define QL_SDMMC_ONLY_USE_DRIVER        0  //0--使用文件系统 1--仅使用sdmmc驱动层
#define QL_SDMMC_USE_1BIT_DATA_SIZE     0  //0--使用默认4bit的数据宽度 1--使用1bit的数据宽度


/*#############################################################################*/


/*===========================================================================
 * Variate
 ===========================================================================*/
#ifdef QL_SDMMC_DET_TEST
ql_task_t ql_sdmmc_det_task = NULL;
#define QL_SDMMC_DET_DEBOUNCE_TIME 100
ql_timer_t ql_sdmmc_det_debounce_timer = NULL;
#endif
/*===========================================================================
 * Functions
 ===========================================================================*/
#ifdef QL_SDMMC_DET_TEST
void ql_sdmmc_det_debounce_callback(void *ctx)
{
	if( ql_sdmmc_det_debounce_timer == NULL || ql_rtos_timer_is_running(ql_sdmmc_det_debounce_timer))
	{
		return;
	}
	ql_rtos_timer_start(ql_sdmmc_det_debounce_timer, QL_SDMMC_DET_DEBOUNCE_TIME, 1);
	QL_SDMMC_DEMO_LOG("sd_det timer start");
}

void ql_sdmmc_det_callback(void *ctx)
{
	ql_event_t ql_event;
	ql_LvlMode sdmmc_det_value;
	ql_event.id = QUEC_SDDET_EVENT_IND;
	
	ql_rtos_timer_stop(ql_sdmmc_det_debounce_timer);
	ql_gpio_get_level(GPIO_15, &sdmmc_det_value);
	if(sdmmc_det_value == LVL_LOW)
	{
		ql_event.param1 = QL_SDMMC_EVENT_INSERT;
		QL_SDMMC_DEMO_LOG("sd detect plug_in");
	}
	else 
	{
		ql_event.param1 = QL_SDMMC_EVENT_PLUGOUT;
		QL_SDMMC_DEMO_LOG("sd detect plug_out ");
	}
	ql_rtos_event_send(ql_sdmmc_det_task, &ql_event);
}

#if 0
//sdmmc供电复位
void _sdmmc_pwd_up_opt()
{
//例如通过某个pin控制是否给SD卡/emmc供电
#define PIN_SDMMC_POWER_NUM  10
#define PORT_SDMMC_POWER_NUM GPIO_20
    //SD卡下电
    //低电平SD卡下电
    ql_gpio_set_level(PORT_SDMMC_POWER_NUM, LVL_LOW);
    ql_rtos_task_sleep_ms(100);
    //sd卡重新上电
    //高电平SD卡上电
    ql_gpio_set_level(PORT_SDMMC_POWER_NUM, LVL_HIGH);
    QL_SDMMC_DEMO_LOG("sdmmc power down and up");
}
#endif

void ql_sdmmc_pin_init(void)
{
    ql_rtos_task_sleep_s(15);
#ifdef QL_SDMMC_DET_TEST
    ql_pin_set_func(QL_SDMMC_PIN_DET    , QL_PIN_SDMMC_MODE_FUNC_GPIO);     //Pin reuse
#endif
	ql_pin_set_func(QL_PIN_SDMMC_CMD    , QL_PIN_SDMMC_MODE_FUNC);     //Pin reuse
	ql_pin_set_func(QL_PIN_SDMMC_DATA_0 , QL_PIN_SDMMC_MODE_FUNC);     //Pin reuse
#if !QL_SDMMC_USE_1BIT_DATA_SIZE
    //使用1bit的数据总线，不需要设置DATA1-DATA3
	ql_pin_set_func(QL_PIN_SDMMC_DATA_1 , QL_PIN_SDMMC_MODE_FUNC);     //Pin reuse
	ql_pin_set_func(QL_PIN_SDMMC_DATA_2 , QL_PIN_SDMMC_MODE_FUNC);     //Pin reuse
	ql_pin_set_func(QL_PIN_SDMMC_DATA_3 , QL_PIN_SDMMC_MODE_FUNC);     //Pin reuse
#endif
	ql_pin_set_func(QL_PIN_SDMMC_CLK    , QL_PIN_SDMMC_MODE_FUNC);     //Pin reuse

#if QL_SDMMC_USE_1BIT_DATA_SIZE
	ql_sdmmc_cfg_t cfg = {0};//先将整个结构体赋初值0，避免后续新增结构体成员因为未赋值导致出现问题
	cfg.dev = QL_SDMMC_EMMC;  //先以emmc流程进行初始化,如果失败再以SD卡流程进行初始化
	cfg.sd_mv = 0;     //SD卡默认电压域3.2v
	cfg.emmc_mv = 0;   //emmc默认电压域1.8v
	cfg.data_size = QL_SDMMC_DATA_SIZE_1BIT;   //使用1bit的数据总线
    ql_sdmmc_set_dev_cfg(cfg);
#endif

#if 0
	ql_sdmmc_cfg_t cfg = {0};//先将整个结构体赋初值0，避免后续新增结构体成员因为未赋值导致出现问题 
    cfg.dev = QL_SDMMC_SD_CARD_ONLY;  //只以sd卡方式进行初始化
    cfg.sd_mv = 0;     //SD卡默认电压域3.2v
    cfg.emmc_mv = 0;   //emmc默认电压域1.8v
    ql_sdmmc_set_dev_cfg(cfg);
	ql_sdmmc_register_power_reset_cb(_sdmmc_pwd_up_opt,true);//注册power reset操作并使能retry功能
#endif
}

ql_errcode_sdmmc_e ql_sdmmc_det_init(void)
{
	/*sd det interrup*/
	if(QL_GPIO_SUCCESS != ql_int_register(GPIO_15, EDGE_TRIGGER, DEBOUNCE_EN, EDGE_BOTH, PULL_UP, ql_sdmmc_det_debounce_callback, NULL))
	{
		QL_SDMMC_DEMO_LOG("det init reg err");
		return QL_SDMMC_INIT_ERR;
	}

    ql_int_enable(GPIO_15);

	return QL_SDMMC_SUCCESS;
}

static void ql_sdmmc_demo_det_thread(void *param)
{
	if(QL_SDMMC_SUCCESS != ql_sdmmc_det_init())
	{
		QL_SDMMC_DEMO_LOG("exit det init err");
		ql_rtos_task_delete(NULL);
	}

	while(1)
	{
		ql_event_t ql_event = {0};
		if(ql_event_try_wait(&ql_event) != 0)
		{
			continue;
		}
		
		if( ql_event.id == QUEC_SDDET_EVENT_IND )
		{
			if(ql_event.param1 == QL_SDMMC_EVENT_INSERT)
			{
			  #if QL_SDMMC_ONLY_USE_DRIVER              
				if(QL_SDMMC_SUCCESS != ql_sdmmc_open())
				{
					QL_SDMMC_DEMO_LOG("det sdmmc open failed");
				}
				else 
				{
					QL_SDMMC_DEMO_LOG("det sdmmc open succeed");
				} 
              #else
				if(QL_SDMMC_SUCCESS != ql_sdmmc_mount())
				{
					QL_SDMMC_DEMO_LOG("det mount failed");
				}
				else 
				{
					QL_SDMMC_DEMO_LOG("det mount succeed");
				}                
              #endif
			}
			else if(ql_event.param1 == QL_SDMMC_EVENT_PLUGOUT)
			{
			  #if QL_SDMMC_DET_MOUNT_OR_DRIVER
				ql_sdmmc_close();
				QL_SDMMC_DEMO_LOG("det sdmmc close succeed");               
              #else
				ql_sdmmc_umount();
				QL_SDMMC_DEMO_LOG("det umount succeed");               
              #endif
			}
		}
	}
}
#endif

#if QL_SDMMC_ONLY_USE_DRIVER == 0
ql_errcode_sdmmc_e ql_sdmmc_mount_demo(void)
{
	if(QL_SDMMC_SUCCESS != ql_sdmmc_mount())
	{
		QL_SDMMC_DEMO_LOG("Mount failed");
		return QL_SDMMC_MOUNT_ERR;
	}
	else 
	{
		QL_SDMMC_DEMO_LOG("Mount succeed");
	}
	//ql_rtos_task_sleep_s(3);
	return QL_SDMMC_SUCCESS;
}
#endif

#if QL_SDMMC_ONLY_USE_DRIVER  //只使用sdmmc驱动

static void ql_sdmmc_demo_thread(void *param)
{
#define SDMMC_SECTOR_SIZE   512
#define SDMMC_OPT_ADDR_ALIN CONFIG_CACHE_LINE_SIZE
    //char wbuffer[512] = {QL_SDMMC_TEST_STR};
    //char rbuffer[512] = {0};
    char *wMollocBufPtr = (char*)calloc(1, SDMMC_SECTOR_SIZE+SDMMC_OPT_ADDR_ALIN);
    if(wMollocBufPtr == NULL)
    {
        QL_SDMMC_DEMO_LOG("calloc err");
        goto calloc_err;
    }
    char *wbuffer = (char *)OSI_ALIGN_UP(wMollocBufPtr, SDMMC_OPT_ADDR_ALIN);
    memcpy(wbuffer,QL_SDMMC_TEST_STR,strlen(QL_SDMMC_TEST_STR));
    
    char *rMollocBufPtr = (char*)calloc(1, SDMMC_SECTOR_SIZE+SDMMC_OPT_ADDR_ALIN);
    if(rMollocBufPtr == NULL)
    {
        QL_SDMMC_DEMO_LOG("calloc err");
        goto calloc_err;
    }    
    char *rbuffer = (char *)OSI_ALIGN_UP(rMollocBufPtr, SDMMC_OPT_ADDR_ALIN); 
    
    uint32_t clk_freq = QL_SDMMC_CLK_FREQ;
    ql_errcode_sdmmc_e ret;

    ret = ql_sdmmc_open();
    if(ret)
    {
       QL_SDMMC_DEMO_LOG("sdmmc open fail:%d",ret);
       goto exit;	
    }

    ql_sdmmc_hw_info_t info = {0};
    ret = ql_sdmmc_get_hw_info(&info);
    if(ret)
    {
       QL_SDMMC_DEMO_LOG("sdmmc get info fail:%d",ret);
       goto exit;	
    }
    //sdmmc初始化 SD1 CID[127-96] [95-64] [63-32] [31-0]
    QL_SDMMC_DEMO_LOG("sdmmc info:mid/0x%0x,pnm/0x%x%x%x%x%x%x,psn/0x%x%x%x%x,blknum/%d,blksize/%d",\
                       info.mid,\
                       info.pnm[0],info.pnm[1],info.pnm[2],info.pnm[3],info.pnm[4],info.pnm[5],\
                       info.psn[0],info.psn[1],info.psn[2],info.psn[3],\
                       info.blknum,\
                       info.blksize);
    
    uint32_t block_num = ql_sdmmc_get_block_number();
    QL_SDMMC_DEMO_LOG("block num:%d",block_num);
    ret = ql_sdmmc_write(QL_SDMMC_BLOCK_NUM, wbuffer, SDMMC_SECTOR_SIZE);
    if(ret)
    {
       QL_SDMMC_DEMO_LOG("sdmmc write fail:%d",ret);
       goto exit;	
    }
    ret = ql_sdmmc_read(QL_SDMMC_BLOCK_NUM, rbuffer, SDMMC_SECTOR_SIZE);
    if(ret)
    {
       QL_SDMMC_DEMO_LOG("sdmmc read fail:%d",ret);
       goto exit;	
    }
    QL_SDMMC_DEMO_LOG("sdmmc read :%s",rbuffer);

    ret = ql_sdmmc_set_clk(clk_freq);
    if(ret)
    {
       QL_SDMMC_DEMO_LOG("sdmmc set clk fail:%d",ret);
       goto exit;	
    }
    QL_SDMMC_DEMO_LOG("sdmmc set clk :%ld",clk_freq); 

exit:
    free(wMollocBufPtr);
    free(rMollocBufPtr);
calloc_err:    
    QL_SDMMC_DEMO_LOG("exit ql_sdmmc_demo_thread");
	ql_rtos_task_delete(NULL);
    
}
#else   //通过文件系统操作sdmmc
#if QL_SDMMC_FS_TEST
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FILE
static int64 ql_sdmmc_fs_test(char* path_name)
{
	int fd = 0;
	int64 err = 0;
	char buffer[100];
	char *str = QL_SDMMC_TEST_STR;   
    
    fd = ql_fopen(path_name, "wb+");
    if(fd < 0)
    {
    	QL_SDMMC_DEMO_LOG("open file failed");
    	err = fd;
    	goto exit;
    }

    err = ql_fwrite(str, strlen(str) + 1, 1, fd);   //strlen not include '\0'
    if(err < 0)
    {
    	QL_SDMMC_DEMO_LOG("write file failed");
    	ql_fclose(fd);
    	goto exit;		
    }

    err = ql_frewind(fd);
    if(err < 0)
    {
    	QL_SDMMC_DEMO_LOG("rewind file failed");
    	ql_fclose(fd);
    	goto exit;					
    }

    err = ql_fread(buffer, ql_fsize(fd), 1, fd);
    if(err < 0)
    {
    	QL_SDMMC_DEMO_LOG("read file failed");
    	ql_fclose(fd);
    	goto exit;					
    }

    QL_SDMMC_DEMO_LOG("file read result is %s", buffer);
    ql_fclose(fd);
exit:
   
    return err;
}
void ql_sdmmc_demo_fs_thread(void *ctx)
{
    int64 err = 0;
#if 0
#if QL_SDMMC_MUTIL_PARTITION_TEST == 0


    //careful format sd, delete all files.
    if(QL_SDMMC_SUCCESS != ql_sdmmc_mkfs(QL_FM_FAT32))
    {
        QL_SDMMC_DEMO_LOG("mkfs failed");
        err =  QL_SDMMC_MKFS_ERR;
        goto exit;
    }
    else 
    {
        QL_SDMMC_DEMO_LOG("mkfs succeed");
    }   
    //ql_rtos_task_sleep_s(2);
#endif
#endif

#if QL_SDMMC_MUTIL_PARTITION_TEST
    if(!ql_sdmmc_is_fdisk_ex())
    {
        ql_sdmmc_part_info_t part_info[2] =
        {
            //如果分区总大小 > 容量，会分区失败
            //注意实际容量会比卡上标识的容量小一点点，所以最后一个分区可以填小一点，会自动调整为剩余容量
            //文件系统是有最小扇区数量要求的，最小为1024个扇区，所以分区容量不能太小。
            {QL_SDMMC_PARTITION_NUM_1,1000},//1000M
            {QL_SDMMC_PARTITION_NUM_2,8}    //8M,实际值会根据容量自动调整为剩余容量
        };
        err = ql_sdmmc_fdisk_ex(part_info);
        if(err != QL_SDMMC_SUCCESS)
        {
            QL_SDMMC_DEMO_LOG("fdisk failed");
            goto exit;
        }
    }
    ql_sdmmc_mount_ex(QL_SDMMC_PARTITION_NUM_ALL);//挂载所有分区
    /*
    for(uint8_t i=QL_SDMMC_PARTITION_NUM_1;i<QL_SDMMC_PARTITION_NUM_MAX;i++)
    {
        err = ql_sdmmc_mount_ex(i);
        if(err != QL_SDMMC_SUCCESS)
        {
            if(!ql_sdmmc_is_format_ex(i))
            {
                err = ql_sdmmc_mkfs_ex(i,QL_FM_FAT32);
                if(err != QL_SDMMC_SUCCESS)
                {
                    QL_SDMMC_DEMO_LOG("mkfs failed");
                    goto exit;
                }
            }
            QL_SDMMC_DEMO_LOG("mount failed");
            goto exit;            
        }
    }*/
    
    err = ql_sdmmc_fs_test(QL_SDMMC_FILE_PATH1); 
#endif
    err = ql_sdmmc_fs_test(QL_SDMMC_FILE_PATH);
    goto exit;//防止报错

exit:	
	if(err < 0)
	{
		QL_SDMMC_DEMO_LOG("errcode is %x", err);
	}
	
	QL_SDMMC_DEMO_LOG("exit ql_sdmmc_demo_fs_thread");
	ql_rtos_task_delete(NULL);
}
#endif
#endif
#endif

void ql_sdmmc_app_init(void)
{
	QlOSStatus err = QL_SDMMC_SUCCESS;

	/*sd pin init*/
	ql_sdmmc_pin_init();

#if QL_SDMMC_ONLY_USE_DRIVER == 0
#if QL_SDMMC_MUTIL_PARTITION_TEST == 0
    err = ql_sdmmc_mount_demo();
    if(err!= QL_SDMMC_SUCCESS)
    {
        QL_SDMMC_DEMO_LOG("sdmmc mount demo fail err = %d", err);
    }
#endif
#endif	

#ifdef QL_SDMMC_DET_TEST
	err = ql_rtos_task_create(&ql_sdmmc_det_task, QL_SDMMC_TASK_STACK_SIZE, QL_SDMMC_TASK_PRIO, "sdDEMO_det", ql_sdmmc_demo_det_thread, NULL, QL_SDMMC_TASK_EVENT_CNT);
	if (err != QL_OSI_SUCCESS)
	{
		QL_SDMMC_DEMO_LOG("creat sd task fail err = %d", err);
	}
	
    err = ql_rtos_timer_create(&ql_sdmmc_det_debounce_timer, ql_sdmmc_det_task, ql_sdmmc_det_callback, NULL);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SDMMC_DEMO_LOG("creat timer task fail err = %d", err);
	}
#endif

#if QL_SDMMC_ONLY_USE_DRIVER
	ql_task_t sdmmc_task = NULL;
	err = ql_rtos_task_create(&sdmmc_task, QL_SDMMC_TASK_STACK_SIZE, QL_SDMMC_TASK_PRIO, "sdDEMO", ql_sdmmc_demo_thread, NULL, QL_SDMMC_TASK_EVENT_CNT);
	if (err != QL_OSI_SUCCESS)
	{
		QL_SDMMC_DEMO_LOG("creat sd task failed err = %d", err);
	}
#else
#if QL_SDMMC_FS_TEST
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FILE
    ql_task_t sdmmc_task_fs = NULL;
	err = ql_rtos_task_create(&sdmmc_task_fs, QL_SDMMC_TASK_STACK_SIZE, QL_SDMMC_TASK_PRIO, "sdDEMO_fs", ql_sdmmc_demo_fs_thread, NULL, QL_SDMMC_TASK_EVENT_CNT);
	if(err != QL_OSI_SUCCESS)
	{
		QL_SDMMC_DEMO_LOG("creat sd task fs failed err = %d", err);
	}
#endif
#endif
#endif

}



