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
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_pin_cfg.h"
#include "power_demo.h"

#ifdef QL_APP_FEATURE_USB
#include "ql_usb.h"
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_POWERDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_POWERDEMO_LOG(msg, ...)         QL_LOG(QL_POWERDEMO_LOG_LEVEL, "ql_POWER", msg, ##__VA_ARGS__)
#define QL_POWERDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_POWER", msg, ##__VA_ARGS__)

#if !defined(require_action)
	#define require_action(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_POWERDEMO_LOG(str);																		\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

/*===========================================================================
 * Variate
 ===========================================================================*/
ql_task_t power_task = NULL;
ql_timer_t power_timer = NULL;
int wake_lock_1, wake_lock_2;

ql_task_t pwrkey_task = NULL;

/* awakening source */
uint resume_src[] = {
    QUEC_RESUME_SRC_PMIC,
    QUEC_RESUME_SRC_VAD, 
    QUEC_RESUME_SRC_KEY, 
    QUEC_RESUME_SRC_GPIO1, 			
    QUEC_RESUME_SRC_UART1, 			
    QUEC_RESUME_SRC_WCN2SYS, 		
    QUEC_RESUME_SRC_WCN_OSC,		
    QUEC_RESUME_SRC_IDLE_TIMER1,	
    QUEC_RESUME_SRC_IDLE_TIMER2, 	
    QUEC_RESUME_SRC_SELF,		
    QUEC_RESUME_SRC_USB_MON, 
};

/*===========================================================================
 * Functions
 ===========================================================================*/
//Sleep callback function is executed before sleep, custom can close some pins to reduce leakage or saving some information in here
//Caution:callback functions cannot run too much code 
void ql_enter_sleep_cb(void* ctx)
{   
    //QL_POWERDEMO_LOG("enter sleep cb");

#ifdef QL_APP_FEATURE_GNSS
    ql_pin_set_func(QL_PIN_NUM_KEYOUT_5, QL_FUN_NUM_UART_2_CTS);  //keyout5 pin need be low level when enter sleep, adjust the function to uart2_rts can do it
    ql_gpio_set_level(GPIO_12, LVL_HIGH);                         //close mos linked to gnss, to avoid high current in sleep mode
    ql_gpio_set_level(GPIO_11, LVL_LOW);                          //gpio11 need be low level when enter sleep to reduce leakage current to gnss
#endif
}

#if 0
/*
    If you get the wake-up source, you can open this interface function.
*/
void ql_wakeup_source_handle(uint all_source)
{
    int i = 0;
    while(i<(sizeof(resume_src)/sizeof(uint)))
    {
        uint source = all_source & resume_src[i];
        i++;
        if(!source)
        {
            continue;
        }
        switch (source)
        {
            /*  The wake-up call source is pmic  */
            /*  The wake-up source of pmic also prints the gpio wake-up source, so you can ignore the gpio wake-up source.  */
            case QUEC_RESUME_SRC_PMIC:
                QL_POWERDEMO_LOG("wakeup from pmic");  
                break;
            
            /*  Not used, no need to pay attention to  */
            case QUEC_RESUME_SRC_VAD:
                QL_POWERDEMO_LOG("wakeup from vad");  
                break;
            
            /*  The wake-up call source is keyboard  */
            case QUEC_RESUME_SRC_KEY:
                QL_POWERDEMO_LOG("wakeup from key");  
                break;

            /*  The wake-up call source is Gpio interrupt  */
            case QUEC_RESUME_SRC_GPIO1:
                QL_POWERDEMO_LOG("wakeup from Gpio interrupt");  
                break;
            
            /*  The wake-up call source is uart1  */
            case QUEC_RESUME_SRC_UART1:
                QL_POWERDEMO_LOG("wakeup from uart1");  
                break;

            /*  The wake-up call source is Bluetooth system  */
            case QUEC_RESUME_SRC_WCN2SYS:
                QL_POWERDEMO_LOG("wakeup from Bluetooth system");  
                break;

            /*  The wake-up call source is Bluetooth osc  */
            case QUEC_RESUME_SRC_WCN_OSC:
                QL_POWERDEMO_LOG("wakeup from Bluetooth osc");  
                break;

            /*  The wake-up call source is cp timer  */
            /* The service on the cp side wakes up, and the specific service cannot be specified. */
            /* It can only be determined by combining with cp log. */
            case QUEC_RESUME_SRC_IDLE_TIMER1:
                QL_POWERDEMO_LOG("wakeup from cp timer");  
                break;

            /*  The wake-up call source is ap timer  */
            /* There are two wake-up, timer, and ql_rtos_task_sleep interfaces */
            /* It is impossible to further determine which awakening */
            case QUEC_RESUME_SRC_IDLE_TIMER2:
                QL_POWERDEMO_LOG("wakeup from ap timer");  
                break;

            /*  Not used, no need to pay attention to  */
            case QUEC_RESUME_SRC_SELF:
                QL_POWERDEMO_LOG("wakeup from self");  
                break;

            /*  Not used, no need to pay attention to  */
            case QUEC_RESUME_SRC_USB_MON:
                QL_POWERDEMO_LOG("wakeup from usb");  
                break;

            default:
                QL_POWERDEMO_LOG("wakeup from default");  
                break;
        }
    }
}
#endif
//exit sleep callback function is executed after exiting sleep, custom can recover the information before sleep
//Caution:callback functions cannot run too much code 
void ql_exit_sleep_cb(void* ctx)
{   
    //QL_POWERDEMO_LOG("exit sleep cb");  
    
#ifdef QL_APP_FEATURE_GNSS
    ql_pin_set_func(QL_PIN_NUM_KEYOUT_5, QL_FUN_NUM_UART_3_TXD);  //keyout5 pin used as gnss uart3_txd function, after exit sleep, set it to uart3_txd
#endif    
    /*
        If you want to get the wake-up source, you can open this interface function.
        This function does not necessarily need to be called in this callback function.
    */
    // uint source = ql_get_wakeup_source();
    // ql_wakeup_source_handle(source);
}

#ifdef QL_APP_FEATURE_USB
int usb_hotplug_cb(QL_USB_HOTPLUG_E state, void *ctx)
{
	if(state == QL_USB_HOTPLUG_OUT)
	{
		QL_POWERDEMO_LOG("USB plug out");
	}
	else
	{
		QL_POWERDEMO_LOG("USB inserted");
	}

	return 0;
}
#endif


static void ql_power_demo_thread(void *param)
{
    //QL_POWERDEMO_LOG("power demo thread enter, param 0x%x", param);

	ql_event_t event;
	int err;

    //register sleep callback function
    ql_sleep_register_cb(ql_enter_sleep_cb);
    
    //register wakeup callback function
    ql_wakeup_register_cb(ql_exit_sleep_cb);

#ifdef QL_APP_FEATURE_USB	
	//register usb hotplug callback function
	ql_usb_bind_hotplug_cb(usb_hotplug_cb);
#endif

	while(1)
	{
		if(ql_event_try_wait(&event) != 0)
		{
			continue;
		}	
		QL_POWERDEMO_LOG("receive event, id is %d", event.id);
		
		switch(event.id)
		{
			case QUEC_SLEEP_ENETR_AUTO_SLEPP:
				
				err = ql_autosleep_enable(QL_ALLOW_SLEEP);
                if( err != 0 )
                {
                    QL_POWERDEMO_LOG("failed to set auto sleep");
                    break;
                }

				err = ql_lpm_wakelock_unlock(wake_lock_1);
                if( err != 0 )
                {
                    QL_POWERDEMO_LOG("lock1 unlocked failed");
                    break;
                }

				err = ql_lpm_wakelock_unlock(wake_lock_2);
                if( err != 0 )
                {
                    QL_POWERDEMO_LOG("lock2 unlocked failed");
                    break;
                }
				
				QL_POWERDEMO_LOG("set auto sleep mode ok");
				
			break;

			case QUEC_SLEEP_EXIT_AUTO_SLEPP:
				err = ql_autosleep_enable(QL_NOT_ALLOW_SLEEP);
                if( err != 0 )
                {
                    QL_POWERDEMO_LOG("failed to set auto sleep");
                    break;
                }

			break;

			case QUEC_SLEEP_QUICK_POWER_DOWM:
				ql_power_down(POWD_IMMDLY);
			break;

			case QUEC_SLEEP_NORMAL_POWER_DOWM:
				ql_power_down(POWD_NORMAL);
			break;

			case QUEC_SLEEP_QUICK_RESET:
				ql_power_reset(RESET_QUICK);
			break;

			case QUEC_SLEEP_NORMAL_RESET:
				ql_power_reset(RESET_NORMAL);
			break;

			default:
			break;
		}
	}

    ql_rtos_task_delete(NULL);
}

void power_timer_callback(void *ctx)
{
	ql_event_t event = {0};

	event.id = QUEC_SLEEP_ENETR_AUTO_SLEPP;
	ql_rtos_event_send(power_task, &event);
}

void ql_power_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&power_task, 1024, APP_PRIORITY_NORMAL, "ql_powerdemo", ql_power_demo_thread, NULL, 3);
	require_action(err, return, "power demo task created failed");

	// err = ql_rtos_timer_create(&power_timer, power_task, power_timer_callback, NULL);
	// require_action(err, return, "demo_timer created failed");

	// err = ql_rtos_timer_start(power_timer, 1000, 0);   // 1秒后开启自动休眠
	// require_action(err, return, "demo_timer start failed");

	// wake_lock_1 = ql_lpm_wakelock_create("my_lock_1", 10);
	// require_action((wake_lock_1 <= 0), return, "lock1 created failed");
	
	// wake_lock_2 = ql_lpm_wakelock_create("my_lock_2", 10);
	// require_action((wake_lock_2 <= 0), return, "lock2 created failed");

	// err = ql_lpm_wakelock_lock(wake_lock_1);
	// require_action(err, return, "lock1 locked failed");

	// err = ql_lpm_wakelock_lock(wake_lock_2);
	// require_action(err, return, "lock2 locked failed");	
}


/*****  pwrkey demo  *****/
static void _pwrkey_demo_callback(void)
{
    ql_event_t event;

    event.id = QUEC_PWRKEY_SHUTDOWN_START_IND;

    ql_rtos_event_send(pwrkey_task, &event);
}

static void _pwrkey_longpress_callback(void)
{
    ql_event_t event;

    event.id = QUEC_PWRKEY_LONGPRESS_IND;

    ql_rtos_event_send(pwrkey_task, &event);
}

static void _pwrkey_press_callback(void)
{
    ql_event_t event;

    event.id = QUEC_PWRKEY_PRESS_IND;

    ql_rtos_event_send(pwrkey_task, &event);
}

static void _pwrkey_release_callback(void)
{
    ql_event_t event;

    event.id = QUEC_PWRKEY_RELEASE_IND;

    ql_rtos_event_send(pwrkey_task, &event);
}

static void ql_pwrkey_demo_thread(void *param)
{
    //QL_POWERDEMO_LOG("pwrkey demo thread enter, param 0x%x", param);

    ql_event_t event;

    ql_pwrkey_shutdown_time_set(3000);                              // long pressed 3s shutdown
    ql_pwrkey_callback_register(_pwrkey_demo_callback);             // long press & release trigger
    ql_pwrkey_longpress_cb_register(_pwrkey_longpress_callback, 7000);    // long press & not release, long pressed 7s trigger
    ql_pwrkey_press_cb_register(_pwrkey_press_callback);
    ql_pwrkey_release_cb_register(_pwrkey_release_callback);

    while(1)
    {
        if(ql_event_try_wait(&event) != 0)
        {
            continue;
        }

        switch(event.id)
        {
            case QUEC_PWRKEY_SHUTDOWN_START_IND:
                QL_POWERDEMO_LOG("customer process");
                /* do something */
                ql_power_down(POWD_NORMAL);
                break;

            case QUEC_PWRKEY_LONGPRESS_IND:
                QL_POWERDEMO_LOG("pwrkey long press trigger");
                /* do something */
                ql_power_reset(RESET_NORMAL);
                break;

            case QUEC_PWRKEY_PRESS_IND:
                QL_POWERDEMO_LOG("pwrkey short press");
                /* do something */
                break;

            case QUEC_PWRKEY_RELEASE_IND:
                QL_POWERDEMO_LOG("pwrkey short release");
                /* do something */
                break;

            default:
                break;
        }
    }

    ql_rtos_task_delete(NULL);
}

void ql_pwrkey_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&pwrkey_task, 1024, APP_PRIORITY_NORMAL, "ql_pwrkeydemo", ql_pwrkey_demo_thread, NULL, 3);
    require_action(err, return, "pwrkey demo task created failed");

}



