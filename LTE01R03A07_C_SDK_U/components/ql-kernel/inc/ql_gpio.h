
/**  @file
  ql_gpio.h

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


#ifndef QL_GPIO_H
#define QL_GPIO_H


#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_GPIO_ERRCODE_BASE (QL_COMPONENT_BSP_GPIO<<16)

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * GPIO errcode
 */
typedef enum
{
    QL_GPIO_SUCCESS = QL_SUCCESS,

    QL_GPIO_EXECUTE_ERR                 = 1|QL_GPIO_ERRCODE_BASE,
    QL_GPIO_INVALID_PARAM_ERR,
    QL_GPIO_OPEN_ERR,
    QL_GPIO_CONFIG_ERR,
    QL_GPIO_CALLBACK_ERR,
    QL_GPIO_LEVEL_TRIGGER_ERR,
    QL_GPIO_MEM_NULL_ERR,
    QL_GPIO_SET_DRVING_ERR,
}ql_errcode_gpio;

/**
 * GPIO number
 */
typedef enum
{
    GPIO_0 = 0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    GPIO_10,
    GPIO_11,
    GPIO_12,
    GPIO_13,
    GPIO_14,
    GPIO_15,
    GPIO_16,
    GPIO_17,
    GPIO_18,
    GPIO_19,
    GPIO_20,
    GPIO_21,
    GPIO_22,
    GPIO_23,
    GPIO_24,
    GPIO_25,
    GPIO_26,
    GPIO_27,
    GPIO_28,
    GPIO_29,
    GPIO_30,
    GPIO_31,
    GPIO_MAX
}ql_GpioNum;

/**
 * GPIO direction
 */
typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT
}ql_GpioDir;

/**
 * GPIO output level
 */
typedef enum
{
    LVL_LOW,
    LVL_HIGH
}ql_LvlMode;

/**
 * GPIO input pull mode
 */
typedef enum
{
    PULL_NONE,
    PULL_DOWN,
    PULL_UP
}ql_PullMode;

/**
 * GPIO interrupt trigger
 */
typedef enum
{
    EDGE_TRIGGER,
    LEVEL_TRIGGER
}ql_TriggerMode;

/**
 * GPIO interrupt debounce
 */
typedef enum
{
    DEBOUNCE_DIS,
    DEBOUNCE_EN
}ql_DebounceMode;

/**
 * GPIO edge-trigger mode
 */
typedef enum
{
    EDGE_RISING,
    EDGE_FALLING,
    EDGE_BOTH
}ql_EdgeMode;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief GPIO initialize
 * @param gpio_num  : GPIO number
 * @param gpio_dir  : GPIO direction
 * @param gpio_pull : GPIO input pull
 * @param gpio_lvl  : GPIO output level
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_OPEN_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_SUCCESS
 * Attention: Before initializing the pins to GPIO, make sure that they match the functional design,  
 * for example, multiplexed pins of the DSDS and DSSS versions SIM2 cannot be initialized to GPIO.
 */
ql_errcode_gpio ql_gpio_init(ql_GpioNum gpio_num, ql_GpioDir gpio_dir, ql_PullMode gpio_pull, ql_LvlMode gpio_lvl);

/**
 * @brief GPIO de-initialize
 * @param gpio_num  : GPIO number
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_deinit(ql_GpioNum gpio_num);

/**
 * @brief GPIO set output level
 * @param gpio_num  : GPIO number
 * @param gpio_lvl  : GPIO set output level
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_set_level(ql_GpioNum gpio_num, ql_LvlMode gpio_lvl);

/**
 * @brief GPIO get output level
 * @param gpio_num  : GPIO number
 * @param gpio_lvl  : GPIO get output level
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_get_level(ql_GpioNum gpio_num, ql_LvlMode *gpio_lvl);

/**
 * @brief GPIO set direction
 * @param gpio_num  : GPIO number
 * @param gpio_dir  : GPIO set direction
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_set_direction(ql_GpioNum gpio_num, ql_GpioDir gpio_dir);

/**
 * @brief GPIO get direction
 * @param gpio_num  : GPIO number
 * @param gpio_dir  : GPIO get direction
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_get_direction(ql_GpioNum gpio_num, ql_GpioDir *gpio_dir);

/**
 * @brief GPIO set pull
 * @param gpio_num  : GPIO number
 * @param gpio_pull : GPIO set input pull
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_set_pull(ql_GpioNum gpio_num, ql_PullMode gpio_pull);

/**
 * @brief GPIO get pull
 * @param gpio_num  : GPIO number
 * @param gpio_pull : GPIO get input pull
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_gpio_get_pull(ql_GpioNum gpio_num, ql_PullMode *gpio_pull);

/**
 * @brief GPIO interrupt register
 * @param gpio_num      : GPIO number
 * @param gpio_trigger  : GPIO interrupt trigger mode
 * @param gpio_debounce : GPIO interrupt trigger debounce enable/disable
 * @param gpio_edge     : GPIO interrupt edge/level trigger configure
 * @param gpio_pull     : GPIO get input pull
 * @param int_cb        : GPIO interrupt cb
 * @param cb_ctx        : GPIO interrupt cb_ctx
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_LEVEL_TRIGGER_ERR
 *       QL_GPIO_OPEN_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_CALLBACK_ERR
 *       QL_GPIO_SUCCESS
 * Attention:
   1. it is forbidden to block the interrupt;
   2. it is forbidden to call Audio start/stop/close， file write/read，CFW（releated to RPC）in interrupt;
   3. it is forbidden to enter critical in interrupt
   4. it is suggested for users to  perform simple operations , or send event(no timeout) to inform your task in interrupt
 */
ql_errcode_gpio ql_int_register(ql_GpioNum gpio_num, ql_TriggerMode gpio_trigger, ql_DebounceMode gpio_debounce,
                                    ql_EdgeMode gpio_edge, ql_PullMode gpio_pull,
                                    void *int_cb, void *cb_ctx);

/**
 * @brief GPIO interrupt unregister
 * @param gpio_num      : GPIO number
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_int_unregister(ql_GpioNum gpio_num);

/**
 * @brief GPIO interrupt enable
 * @param gpio_num : GPIO number
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_CALLBACK_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_int_enable(ql_GpioNum gpio_num);

/**
 * @brief GPIO interrupt disable
 * @param gpio_num : GPIO number
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_int_disable(ql_GpioNum gpio_num);

/**
 * @brief GPIO interrupt register and enable.
 *        The ql_int_enable_ex will register and enable the GPIO interrupt.
 *        It includes the process in ql_int_register and ql_int_enable,in order to handle the level trigger.
 * @param gpio_num      : GPIO number
 * @param gpio_trigger  : GPIO interrupt trigger mode
 * @param gpio_debounce : GPIO interrupt trigger debounce enable/disable
 * @param gpio_edge     : GPIO interrupt edge/level trigger configure
 * @param gpio_pull     : GPIO get input pull
 * @param int_cb        : GPIO interrupt cb
 * @param cb_ctx        : GPIO interrupt cb_ctx
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_LEVEL_TRIGGER_ERR
 *       QL_GPIO_OPEN_ERR
 *       QL_GPIO_CONFIG_ERR
 *       QL_GPIO_CALLBACK_ERR
 *       QL_GPIO_SUCCESS
 * Attention:
   1. it is forbidden to block the interrupt;
   2. it is forbidden to call Audio start/stop/close， file write/read，CFW（releated to RPC）in interrupt;
   3. it is forbidden to enter critical in interrupt
   4. it is suggested for users to  perform simple operations , or send event(no timeout) to inform your task in interrupt
 */
ql_errcode_gpio ql_int_enable_ex(ql_GpioNum gpio_num, ql_TriggerMode gpio_trigger, ql_DebounceMode gpio_debounce,
                                    ql_EdgeMode gpio_edge, ql_PullMode gpio_pull,
                                    void *int_cb, void *cb_ctx);

/**
 * @brief GPIO interrupt unregister and disable.
 *        It includes the process in ql_int_unregister and ql_int_disable,in order to handle the level trigger.
 * @param gpio_num : GPIO number
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_int_disable_ex(ql_GpioNum gpio_num);

/**
 * @brief Pin function seclect
 * @param pin_num  : Pin number
 * @param func_sel : seclected Pin function
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_pin_set_func(uint8_t pin_num, uint8_t func_sel);

/**
 * @brief Seclected Pin function get
 * @param pin_num  : Pin number
 * @param func_sel : seclected Pin function
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_pin_get_func(uint8_t pin_num, uint8_t *func_sel);

/**
 * @brief Pin function set to GPIO
 * @param pin_num  : Pin number
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 * Attention: Before setting a pin to a gpio function, make sure it matches the function design,
 * for example, multiplexed pins of the DSDS and DSSS versions SIM2 cannot be set to GPIO.
 */
ql_errcode_gpio ql_pin_set_gpio(uint8_t pin_num);

/**
 * @brief set pin drive capability 
 * @param pin_num  : Pin number
 * @param drving   : drive capability：2bit:  0-2ma，1-4ma，2-6ma，3-8ma,
                                       4bit:  0-2.4ma，1-5.5ma，2-7.5ma，3-10.4ma, 4-12.4ma，5-15ma，6-17.4ma，7-20ma,
                                              8-24.6ma，9-27.8ma，10-29.6ma，11-32ma, 12-34.2ma，13-37ma，14-39ma，15-42ma
 * @return
 *       QL_GPIO_INVALID_PARAM_ERR
 *       QL_GPIO_SUCCESS
 */
ql_errcode_gpio ql_pin_set_drving(uint8_t pin_num, char drving);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_GPIO_H */

