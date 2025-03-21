
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QUEC_PIN_INDEX_H
#define QUEC_PIN_INDEX_H


#ifdef __cplusplus
extern "C" {
#endif


#define QUEC_PIN_NONE    0xff

//后为定制两个型号，800G,600G,使用的引脚不同
//QDORIND_PIN---给对端mcu信号的引脚
//QDORIND_WAKEUP_PIN---唤醒引脚，但唤醒逻辑与公版的相反
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef __QUEC_OEM_VER_HW__
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QDORIND_PIN         20
#define QDORIND_WAKEUP_PIN  86
#else
#define QDORIND_PIN         50
#define QDORIND_WAKEUP_PIN  49
#endif
#endif
#endif


/*===========================================================================
 * Pin Number
 ===========================================================================*/
/************    GPIO Default Name    ************/
/************    use Function0 Name   ************///don't change
#define QUEC_GPIO_DNAME_GPIO_0           0
#define QUEC_GPIO_DNAME_SPI_LCD_SIO      0
#define QUEC_GPIO_DNAME_GPIO_1           1
#define QUEC_GPIO_DNAME_SPI_LCD_SDC      1
#define QUEC_GPIO_DNAME_GPIO_2           2
#define QUEC_GPIO_DNAME_SPI_LCD_CLK      2
#define QUEC_GPIO_DNAME_GPIO_3           3
#define QUEC_GPIO_DNAME_SPI_LCD_CS       3
#define QUEC_GPIO_DNAME_GPIO_4           4
#define QUEC_GPIO_DNAME_SPI_LCD_SELECT   4
#define QUEC_GPIO_DNAME_GPIO_5           5
#define QUEC_GPIO_DNAME_LCD_FMARK        5
#define QUEC_GPIO_DNAME_GPIO_7           7
#define QUEC_GPIO_DNAME_GPIO_8           8
#define QUEC_GPIO_DNAME_AP_JTAG_TCK      8
#define QUEC_GPIO_DNAME_GPIO_9           9
#define QUEC_GPIO_DNAME_AP_JTAG_TRST     9
#define QUEC_GPIO_DNAME_GPIO_10          10
#define QUEC_GPIO_DNAME_AP_JTAG_TMS      10
#define QUEC_GPIO_DNAME_GPIO_11          11
#define QUEC_GPIO_DNAME_AP_JTAG_TDI      11
#define QUEC_GPIO_DNAME_GPIO_12          12
#define QUEC_GPIO_DNAME_AP_JTAG_TDO      12
#define QUEC_GPIO_DNAME_GPIO_13          13
#define QUEC_GPIO_DNAME_GPIO_14          14
#define QUEC_GPIO_DNAME_GPIO_15          15
#define QUEC_GPIO_DNAME_I2C_M1_SCL       16
#define QUEC_GPIO_DNAME_I2C_M1_SDA       17
#define QUEC_GPIO_DNAME_GPIO_18          18
#define QUEC_GPIO_DNAME_CAMERA_RST_L     18
#define QUEC_GPIO_DNAME_GPIO_19          19
#define QUEC_GPIO_DNAME_CAMERA_PWDN      19
#define QUEC_GPIO_DNAME_GPIO_20          20
#define QUEC_GPIO_DNAME_CAMERA_REF_CLK   20
#define QUEC_GPIO_DNAME_GPIO_21          21
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_0  21
#define QUEC_GPIO_DNAME_GPIO_22          22
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_1  22
#define QUEC_GPIO_DNAME_GPIO_23          23
#define QUEC_GPIO_DNAME_SPI_CAMERA_SCK   23
#define QUEC_GPIO_DNAME_SDMMC1_CMD       24
#define QUEC_GPIO_DNAME_SDMMC1_DATA_0    25
#define QUEC_GPIO_DNAME_SDMMC1_DATA_1    26
#define QUEC_GPIO_DNAME_SDMMC1_DATA_2    27
#define QUEC_GPIO_DNAME_SDMMC1_DATA_3    28
#define QUEC_GPIO_DNAME_SIM_2_CLK        29
#define QUEC_GPIO_DNAME_SIM_2_DIO        30
#define QUEC_GPIO_DNAME_SIM_2_RST        31
/************ End of GPIO Default Name ************/


/************    Pin Default Name     ************/
/************    use Function0 Name   ************///add pin number for each project
#ifdef CONFIG_QL_PROJECT_DEF_EC200U

/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (53)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            27
#define QUEC_PIN_DNAME_SPI_LCD_SIO       125
#define QUEC_PIN_DNAME_GPIO_1            26
#define QUEC_PIN_DNAME_SPI_LCD_SDC       124
#define QUEC_PIN_DNAME_GPIO_2            24
#define QUEC_PIN_DNAME_SPI_LCD_CLK       123
#define QUEC_PIN_DNAME_GPIO_3            25
#define QUEC_PIN_DNAME_SPI_LCD_CS        122
#define QUEC_PIN_DNAME_GPIO_4            13
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    121
#define QUEC_PIN_DNAME_GPIO_5            135
#define QUEC_PIN_DNAME_LCD_FMARK         119
#define QUEC_PIN_DNAME_GPIO_7            133
#define QUEC_PIN_DNAME_GPIO_8            3
#define QUEC_PIN_DNAME_AP_JTAG_TCK       134
#define QUEC_PIN_DNAME_GPIO_9            40
#define QUEC_PIN_DNAME_AP_JTAG_TRST      132
#define QUEC_PIN_DNAME_GPIO_10           37
#define QUEC_PIN_DNAME_AP_JTAG_TMS       131
#define QUEC_PIN_DNAME_GPIO_11           38
#define QUEC_PIN_DNAME_AP_JTAG_TDI       130
#define QUEC_PIN_DNAME_GPIO_12           39
#define QUEC_PIN_DNAME_AP_JTAG_TDO       129
#define QUEC_PIN_DNAME_GPIO_13           5
#define QUEC_PIN_DNAME_GPIO_14           141
#define QUEC_PIN_DNAME_GPIO_15           142
#define QUEC_PIN_DNAME_I2C_M1_SCL        41
#define QUEC_PIN_DNAME_I2C_M1_SDA        42
#define QUEC_PIN_DNAME_GPIO_18           65
#define QUEC_PIN_DNAME_CAMERA_RST_L      61
#define QUEC_PIN_DNAME_GPIO_19           64
#define QUEC_PIN_DNAME_CAMERA_PWDN       62
#define QUEC_PIN_DNAME_GPIO_20           136
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    63
#define QUEC_PIN_DNAME_GPIO_21           139
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   66
#define QUEC_PIN_DNAME_GPIO_22           126
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   6
#define QUEC_PIN_DNAME_GPIO_23           127
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    23
#define QUEC_PIN_DNAME_SDMMC1_CMD        33
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     31
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     30
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     29
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     28
#define QUEC_PIN_DNAME_SIM_2_CLK         1
#define QUEC_PIN_DNAME_SIM_2_DIO         2
#define QUEC_PIN_DNAME_SIM_2_RST         4
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        32
#define QUEC_PIN_DNAME_KEYOUT_4          81
#define QUEC_PIN_DNAME_KEYOUT_5          82
#define QUEC_PIN_DNAME_KEYIN_4           137
#define QUEC_PIN_DNAME_KEYIN_5           138

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_KEYIN_5

#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_KEYOUT_5
/***********    EC200U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EC600U

/*************** Pin ****************/
#define QUEC_PIN_CFG_MAX    (53)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            61
#define QUEC_PIN_DNAME_SPI_LCD_SIO       66
#define QUEC_PIN_DNAME_GPIO_1            58
#define QUEC_PIN_DNAME_SPI_LCD_SDC       63
#define QUEC_PIN_DNAME_GPIO_2            59
#define QUEC_PIN_DNAME_SPI_LCD_CLK       67
#define QUEC_PIN_DNAME_GPIO_3            60
#define QUEC_PIN_DNAME_SPI_LCD_CS        65
#define QUEC_PIN_DNAME_GPIO_4            69
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    137
#define QUEC_PIN_DNAME_GPIO_5            70
#define QUEC_PIN_DNAME_LCD_FMARK         62
#define QUEC_PIN_DNAME_GPIO_7            118
#define QUEC_PIN_DNAME_GPIO_8            9
#define QUEC_PIN_DNAME_AP_JTAG_TCK       99
#define QUEC_PIN_DNAME_GPIO_9            1
#define QUEC_PIN_DNAME_AP_JTAG_TRST      98
#define QUEC_PIN_DNAME_GPIO_10           4
#define QUEC_PIN_DNAME_AP_JTAG_TMS       95
#define QUEC_PIN_DNAME_GPIO_11           3
#define QUEC_PIN_DNAME_AP_JTAG_TDI       119
#define QUEC_PIN_DNAME_GPIO_12           2
#define QUEC_PIN_DNAME_AP_JTAG_TDO       100
#define QUEC_PIN_DNAME_GPIO_13           54
#define QUEC_PIN_DNAME_GPIO_14           57
#define QUEC_PIN_DNAME_GPIO_15           56
#define QUEC_PIN_DNAME_I2C_M1_SCL        11
#define QUEC_PIN_DNAME_I2C_M1_SDA        12
#define QUEC_PIN_DNAME_GPIO_18           34
#define QUEC_PIN_DNAME_CAMERA_RST_L      120
#define QUEC_PIN_DNAME_GPIO_19           33
#define QUEC_PIN_DNAME_CAMERA_PWDN       16
#define QUEC_PIN_DNAME_GPIO_20           123
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    10
#define QUEC_PIN_DNAME_GPIO_21           124
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   14
#define QUEC_PIN_DNAME_GPIO_22           122
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   15
#define QUEC_PIN_DNAME_GPIO_23           121
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    13
#define QUEC_PIN_DNAME_SDMMC1_CMD        48
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     39
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     40
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     49
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     50
#define QUEC_PIN_DNAME_SIM_2_CLK         53
#define QUEC_PIN_DNAME_SIM_2_DIO         52
#define QUEC_PIN_DNAME_SIM_2_RST         51
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        132
#define QUEC_PIN_DNAME_KEYOUT_4          104
#define QUEC_PIN_DNAME_KEYOUT_5          103
#define QUEC_PIN_DNAME_KEYIN_4           126
#define QUEC_PIN_DNAME_KEYIN_5           125

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_UART2_RTS         QUEC_PIN_DNAME_GPIO_22    //unisoc func is CTS
#define QUEC_PIN_UART2_CTS         QUEC_PIN_DNAME_GPIO_23    //unisoc func is RTS

#ifndef __QUEC_OEM_VER_LD__
//#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_KEYOUT_4
//#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_KEYOUT_5
//#define QUEC_PIN_UART3_RTS         QUEC_PIN_DNAME_KEYIN_4
//#define QUEC_PIN_UART3_CTS         QUEC_PIN_DNAME_KEYIN_5
#else
#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QUEC_PIN_UART3_RTS         QUEC_PIN_DNAME_SDMMC1_DATA_0    //unisoc func is CTS
#define QUEC_PIN_UART3_CTS         QUEC_PIN_DNAME_SDMMC1_DATA_1    //unisoc func is RTS
#endif
/***********    EC600U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG700U

/*************** Pin ****************/
#define QUEC_PIN_CFG_MAX    (43)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            34
#define QUEC_PIN_DNAME_SPI_LCD_SIO       84
#define QUEC_PIN_DNAME_GPIO_1            35
#define QUEC_PIN_DNAME_SPI_LCD_SDC       85
#define QUEC_PIN_DNAME_GPIO_2            77
#define QUEC_PIN_DNAME_SPI_LCD_CLK       43
#define QUEC_PIN_DNAME_GPIO_3            78
#define QUEC_PIN_DNAME_SPI_LCD_CS        42
#define QUEC_PIN_DNAME_GPIO_4            79
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    81
#define QUEC_PIN_DNAME_GPIO_5            80
#define QUEC_PIN_DNAME_LCD_FMARK         82
#define QUEC_PIN_DNAME_GPIO_7            46
#define QUEC_PIN_DNAME_GPIO_8            55
//#define QUEC_PIN_DNAME_AP_JTAG_TCK
#define QUEC_PIN_DNAME_GPIO_9            17
//#define QUEC_PIN_DNAME_AP_JTAG_TRST
#define QUEC_PIN_DNAME_GPIO_10           16
//#define QUEC_PIN_DNAME_AP_JTAG_TMS
#define QUEC_PIN_DNAME_GPIO_11           19
//#define QUEC_PIN_DNAME_AP_JTAG_TDI
#define QUEC_PIN_DNAME_GPIO_12           18
//#define QUEC_PIN_DNAME_AP_JTAG_TDO
#define QUEC_PIN_DNAME_GPIO_13           45
#define QUEC_PIN_DNAME_GPIO_14           86
#define QUEC_PIN_DNAME_GPIO_15           87
#define QUEC_PIN_DNAME_I2C_M1_SCL        28
#define QUEC_PIN_DNAME_I2C_M1_SDA        29
#define QUEC_PIN_DNAME_GPIO_18           1
#define QUEC_PIN_DNAME_CAMERA_RST_L      75
#define QUEC_PIN_DNAME_GPIO_19           90
#define QUEC_PIN_DNAME_CAMERA_PWDN       98
#define QUEC_PIN_DNAME_GPIO_20           64
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    99
#define QUEC_PIN_DNAME_GPIO_21           63
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   76
#define QUEC_PIN_DNAME_GPIO_22           20
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   100
#define QUEC_PIN_DNAME_GPIO_23           22
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    101
#define QUEC_PIN_DNAME_SDMMC1_CMD        21
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     72
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     73
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     69
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     68
//#define QUEC_PIN_DNAME_SIM_2_CLK
//#define QUEC_PIN_DNAME_SIM_2_DIO
//#define QUEC_PIN_DNAME_SIM_2_RST
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        66
#define QUEC_PIN_DNAME_KEYOUT_4          24
#define QUEC_PIN_DNAME_KEYOUT_5          23
//#define QUEC_PIN_DNAME_KEYIN_4
//#define QUEC_PIN_DNAME_KEYIN_5

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21

#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_KEYOUT_5
/***********    EG700U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG500U

/*************** Pin ****************/
#define QUEC_PIN_CFG_MAX    (44)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            46
#define QUEC_PIN_DNAME_SPI_LCD_SIO       19
#define QUEC_PIN_DNAME_GPIO_1            13
#define QUEC_PIN_DNAME_SPI_LCD_SDC       24
#define QUEC_PIN_DNAME_GPIO_2            45
#define QUEC_PIN_DNAME_SPI_LCD_CLK       20
#define QUEC_PIN_DNAME_GPIO_3            44
#define QUEC_PIN_DNAME_SPI_LCD_CS        25
#define QUEC_PIN_DNAME_GPIO_4            54
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    26
#define QUEC_PIN_DNAME_GPIO_5            17
#define QUEC_PIN_DNAME_LCD_FMARK         27
#define QUEC_PIN_DNAME_GPIO_7            55
#define QUEC_PIN_DNAME_GPIO_8            56
//#define QUEC_PIN_DNAME_AP_JTAG_TCK
//#define QUEC_PIN_DNAME_GPIO_9   
//#define QUEC_PIN_DNAME_AP_JTAG_TRST
//EG500U不支持GNSS，定义GPIO_10/GPIO_11/GPIO_12只是为了多合一版本可以编译通过
#define QUEC_PIN_DNAME_GPIO_10     QUEC_PIN_NONE
//#define QUEC_PIN_DNAME_AP_JTAG_TMS
#define QUEC_PIN_DNAME_GPIO_11     QUEC_PIN_NONE
//#define QUEC_PIN_DNAME_AP_JTAG_TDI
#define QUEC_PIN_DNAME_GPIO_12     QUEC_PIN_NONE
//#define QUEC_PIN_DNAME_AP_JTAG_TDO
#define QUEC_PIN_DNAME_GPIO_13           53
#define QUEC_PIN_DNAME_GPIO_14           35
#define QUEC_PIN_DNAME_GPIO_15           36
#define QUEC_PIN_DNAME_I2C_M1_SCL        29
#define QUEC_PIN_DNAME_I2C_M1_SDA        28
#define QUEC_PIN_DNAME_GPIO_18           14
#define QUEC_PIN_DNAME_CAMERA_RST_L      85
#define QUEC_PIN_DNAME_GPIO_19           11
#define QUEC_PIN_DNAME_CAMERA_PWDN       84
#define QUEC_PIN_DNAME_GPIO_20           30
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    83
#define QUEC_PIN_DNAME_GPIO_21           31
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   86
#define QUEC_PIN_DNAME_GPIO_22           32
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   87
#define QUEC_PIN_DNAME_GPIO_23           33
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    88
#define QUEC_PIN_DNAME_SDMMC1_CMD        81
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     90
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     97
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     101
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     111
#define QUEC_PIN_DNAME_SIM_2_CLK         120
#define QUEC_PIN_DNAME_SIM_2_DIO         72
#define QUEC_PIN_DNAME_SIM_2_RST         43
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        80
#define QUEC_PIN_DNAME_KEYOUT_4          75
#define QUEC_PIN_DNAME_KEYOUT_5          76
#define QUEC_PIN_DNAME_KEYIN_4           78
#define QUEC_PIN_DNAME_KEYIN_5           79

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_UART2_RTS         QUEC_PIN_DNAME_GPIO_22    //unisoc func is CTS
#define QUEC_PIN_UART2_CTS         QUEC_PIN_DNAME_GPIO_23    //unisoc func is RTS

//#define QUEC_PIN_UART3_RXD                                        //haven't this function in Standard
//#define QUEC_PIN_UART3_TXD                                        //haven't this function in Standard
//#define QUEC_PIN_UART3_RTS                                        //haven't this function in Standard
//#define QUEC_PIN_UART3_CTS                                        //haven't this function in Standard
/***********    EG500U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG915U

/*************** Pin ****************/
#define QUEC_PIN_CFG_MAX    (45)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            4
#define QUEC_PIN_DNAME_SPI_LCD_SIO       116
#define QUEC_PIN_DNAME_GPIO_1            5
#define QUEC_PIN_DNAME_SPI_LCD_SDC       106
#define QUEC_PIN_DNAME_GPIO_2            6
#define QUEC_PIN_DNAME_SPI_LCD_CLK       105
#define QUEC_PIN_DNAME_GPIO_3            7
#define QUEC_PIN_DNAME_SPI_LCD_CS        16
#define QUEC_PIN_DNAME_GPIO_4            1
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    108
#define QUEC_PIN_DNAME_GPIO_5            20
#define QUEC_PIN_DNAME_LCD_FMARK         92
#define QUEC_PIN_DNAME_GPIO_7            65
#define QUEC_PIN_DNAME_GPIO_8            104
#define QUEC_PIN_DNAME_AP_JTAG_TCK       66
#define QUEC_PIN_DNAME_GPIO_9            26
//#define QUEC_PIN_DNAME_AP_JTAG_TRST      11
#define QUEC_PIN_DNAME_GPIO_10           25
//#define QUEC_PIN_DNAME_AP_JTAG_TMS       12
#define QUEC_PIN_DNAME_GPIO_11           64
//#define QUEC_PIN_DNAME_AP_JTAG_TDI       13
#define QUEC_PIN_DNAME_GPIO_12           88
//#define QUEC_PIN_DNAME_AP_JTAG_TDO       14
#define QUEC_PIN_DNAME_GPIO_13           21
#define QUEC_PIN_DNAME_GPIO_14           40
#define QUEC_PIN_DNAME_GPIO_15           41
#define QUEC_PIN_DNAME_I2C_M1_SCL        103
#define QUEC_PIN_DNAME_I2C_M1_SDA        114
#define QUEC_PIN_DNAME_GPIO_18           37
#define QUEC_PIN_DNAME_CAMERA_RST_L      111
#define QUEC_PIN_DNAME_GPIO_19           36
#define QUEC_PIN_DNAME_CAMERA_PWDN       115
#define QUEC_PIN_DNAME_GPIO_20           28
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    95
#define QUEC_PIN_DNAME_GPIO_21           27
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   97
#define QUEC_PIN_DNAME_GPIO_22           83
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   98
#define QUEC_PIN_DNAME_GPIO_23           42
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    78
#define QUEC_PIN_DNAME_SDMMC1_CMD        38
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     30
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     39
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     18
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     19
#define QUEC_PIN_DNAME_SIM_2_CLK         84
#define QUEC_PIN_DNAME_SIM_2_DIO         86
#define QUEC_PIN_DNAME_SIM_2_RST         85
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        110

#define QUEC_PIN_DNAME_KEYOUT_5          QUEC_PIN_NONE

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21

#define QUEC_PIN_UART1_CTS         QUEC_PIN_DNAME_GPIO_19
/***********    EG915U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG070U

/*************** Pin ****************/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG /* Standard */
#define QUEC_PIN_CFG_MAX    (46)     /* multiple pins amount */
#else
#define QUEC_PIN_CFG_MAX    (51)     /* multiple pins amount */
#endif

#define QUEC_PIN_DNAME_GPIO_0            34
#define QUEC_PIN_DNAME_SPI_LCD_SIO       84
#define QUEC_PIN_DNAME_GPIO_1            35
#define QUEC_PIN_DNAME_SPI_LCD_SDC       85
#define QUEC_PIN_DNAME_GPIO_2            77
#define QUEC_PIN_DNAME_SPI_LCD_CLK       43
#define QUEC_PIN_DNAME_GPIO_3            78
#define QUEC_PIN_DNAME_SPI_LCD_CS        42
#define QUEC_PIN_DNAME_GPIO_4            79
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    81
#define QUEC_PIN_DNAME_GPIO_5            80
#define QUEC_PIN_DNAME_LCD_FMARK         82
#define QUEC_PIN_DNAME_GPIO_7            123
#define QUEC_PIN_DNAME_GPIO_8            55
#define QUEC_PIN_DNAME_AP_JTAG_TCK       124
#define QUEC_PIN_DNAME_GPIO_9            17
#define QUEC_PIN_DNAME_AP_JTAG_TRST      125
#define QUEC_PIN_DNAME_GPIO_10           16
#define QUEC_PIN_DNAME_AP_JTAG_TMS       126
#define QUEC_PIN_DNAME_GPIO_11           19
#define QUEC_PIN_DNAME_AP_JTAG_TDI       127
#define QUEC_PIN_DNAME_GPIO_12           18
#define QUEC_PIN_DNAME_AP_JTAG_TDO       128
#define QUEC_PIN_DNAME_GPIO_13           45
#define QUEC_PIN_DNAME_GPIO_14           86
#define QUEC_PIN_DNAME_GPIO_15           87
#define QUEC_PIN_DNAME_I2C_M1_SCL        28
#define QUEC_PIN_DNAME_I2C_M1_SDA        29
#define QUEC_PIN_DNAME_GPIO_18           1
#define QUEC_PIN_DNAME_CAMERA_RST_L      75
#define QUEC_PIN_DNAME_GPIO_19           90
#define QUEC_PIN_DNAME_CAMERA_PWDN       98
#define QUEC_PIN_DNAME_GPIO_20           64
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    99
#define QUEC_PIN_DNAME_GPIO_21           63
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   76
#define QUEC_PIN_DNAME_GPIO_22           20
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   100
#define QUEC_PIN_DNAME_GPIO_23           22
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    101
#define QUEC_PIN_DNAME_SDMMC1_CMD        21
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     72
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     73
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     69
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     68
#define QUEC_PIN_DNAME_SIM_2_CLK         129
#define QUEC_PIN_DNAME_SIM_2_DIO         130
#define QUEC_PIN_DNAME_SIM_2_RST         132
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        66
#define QUEC_PIN_DNAME_KEYOUT_4          24
#define QUEC_PIN_DNAME_KEYOUT_5          23
//#define QUEC_PIN_DNAME_KEYIN_4
//#define QUEC_PIN_DNAME_KEYIN_5

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21

#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_KEYOUT_5
/***********    EG070U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EC020U

/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (53)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            27
#define QUEC_PIN_DNAME_SPI_LCD_SIO       125
#define QUEC_PIN_DNAME_GPIO_1            26
#define QUEC_PIN_DNAME_SPI_LCD_SDC       124
#define QUEC_PIN_DNAME_GPIO_2            24
#define QUEC_PIN_DNAME_SPI_LCD_CLK       123
#define QUEC_PIN_DNAME_GPIO_3            25
#define QUEC_PIN_DNAME_SPI_LCD_CS        122
#define QUEC_PIN_DNAME_GPIO_4            141
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    121
#define QUEC_PIN_DNAME_GPIO_5            142
#define QUEC_PIN_DNAME_LCD_FMARK         119
#define QUEC_PIN_DNAME_GPIO_7            133
#define QUEC_PIN_DNAME_GPIO_8            61
#define QUEC_PIN_DNAME_AP_JTAG_TCK       134
#define QUEC_PIN_DNAME_GPIO_9            40
#define QUEC_PIN_DNAME_AP_JTAG_TRST      132
#define QUEC_PIN_DNAME_GPIO_10           37
#define QUEC_PIN_DNAME_AP_JTAG_TMS       131
#define QUEC_PIN_DNAME_GPIO_11           38
#define QUEC_PIN_DNAME_AP_JTAG_TDI       130
#define QUEC_PIN_DNAME_GPIO_12           39
#define QUEC_PIN_DNAME_AP_JTAG_TDO       129
#define QUEC_PIN_DNAME_GPIO_13           5
#define QUEC_PIN_DNAME_GPIO_14           13
#define QUEC_PIN_DNAME_GPIO_15           23
#define QUEC_PIN_DNAME_I2C_M1_SCL        41
#define QUEC_PIN_DNAME_I2C_M1_SDA        42
#define QUEC_PIN_DNAME_GPIO_18           64
#define QUEC_PIN_DNAME_CAMERA_RST_L      18
#define QUEC_PIN_DNAME_GPIO_19           65
#define QUEC_PIN_DNAME_CAMERA_PWDN       6
#define QUEC_PIN_DNAME_GPIO_20           66
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    139
#define QUEC_PIN_DNAME_GPIO_21           63
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   136
#define QUEC_PIN_DNAME_GPIO_22           116
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   62
#define QUEC_PIN_DNAME_GPIO_23           126
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    3
#define QUEC_PIN_DNAME_SDMMC1_CMD        33
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     31
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     30
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     29
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     28
#define QUEC_PIN_DNAME_SIM_2_CLK         1
#define QUEC_PIN_DNAME_SIM_2_DIO         2
#define QUEC_PIN_DNAME_SIM_2_RST         4
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SDMMC1_CLK        32
#define QUEC_PIN_DNAME_KEYOUT_4          137
#define QUEC_PIN_DNAME_KEYOUT_5          138
#define QUEC_PIN_DNAME_KEYIN_4           81
#define QUEC_PIN_DNAME_KEYIN_5           82

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21

#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_KEYOUT_5
/***********    EC020U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG912U

/*************** Pin ****************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS

#ifdef CONFIG_GPIO_USED_FOR_VBAT_RF_SWITCH
#define QUEC_PIN_CFG_MAX    (51)     /* multiple pins amount */
#else
#define QUEC_PIN_CFG_MAX    (50)     /* multiple pins amount */
#endif
#else

#ifdef CONFIG_GPIO_USED_FOR_VBAT_RF_SWITCH
#define QUEC_PIN_CFG_MAX    (46)     /* multiple pins amount */
#else
#define QUEC_PIN_CFG_MAX    (45)     /* multiple pins amount */
#endif

#endif

#define QUEC_PIN_DNAME_GPIO_0            4
#define QUEC_PIN_DNAME_SPI_LCD_SIO       116
#define QUEC_PIN_DNAME_GPIO_1            5
#define QUEC_PIN_DNAME_SPI_LCD_SDC       106
#define QUEC_PIN_DNAME_GPIO_2            6
#define QUEC_PIN_DNAME_SPI_LCD_CLK       105
#define QUEC_PIN_DNAME_GPIO_3            7
#define QUEC_PIN_DNAME_SPI_LCD_CS        16
#define QUEC_PIN_DNAME_GPIO_4            1
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    108
#define QUEC_PIN_DNAME_GPIO_5            20
#define QUEC_PIN_DNAME_LCD_FMARK         92
#define QUEC_PIN_DNAME_GPIO_7            65
#define QUEC_PIN_DNAME_GPIO_8            104
#define QUEC_PIN_DNAME_AP_JTAG_TCK       66
#define QUEC_PIN_DNAME_GPIO_9            26
//#define QUEC_PIN_DNAME_AP_JTAG_TRST      11
#define QUEC_PIN_DNAME_GPIO_10           25
//#define QUEC_PIN_DNAME_AP_JTAG_TMS       12
#define QUEC_PIN_DNAME_GPIO_11           64
//#define QUEC_PIN_DNAME_AP_JTAG_TDI       13
#define QUEC_PIN_DNAME_GPIO_12           88
//#define QUEC_PIN_DNAME_AP_JTAG_TDO       14
#define QUEC_PIN_DNAME_GPIO_13           21
#define QUEC_PIN_DNAME_GPIO_14           40
#define QUEC_PIN_DNAME_GPIO_15           41
#define QUEC_PIN_DNAME_I2C_M1_SDA        114
#define QUEC_PIN_DNAME_GPIO_18           37
#define QUEC_PIN_DNAME_CAMERA_RST_L      111
#define QUEC_PIN_DNAME_GPIO_19           36
#define QUEC_PIN_DNAME_CAMERA_PWDN       115
#define QUEC_PIN_DNAME_GPIO_20           28
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    95
#define QUEC_PIN_DNAME_GPIO_21           27
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   97
#define QUEC_PIN_DNAME_GPIO_22           83
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   98
#define QUEC_PIN_DNAME_GPIO_23           42
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    78
#define QUEC_PIN_DNAME_SDMMC1_CMD        38
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     30
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     39
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     18
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     19
#define QUEC_PIN_DNAME_SIM_2_CLK         84
#define QUEC_PIN_DNAME_SIM_2_DIO         86
#define QUEC_PIN_DNAME_SIM_2_RST         85
//pinmux(not gpio part)
#define QUEC_PIN_DNAME_I2C_M1_SCL        103
#define QUEC_PIN_DNAME_SDMMC1_CLK        110

//以下引脚保留给模块使用，客户无法使用，且不能修改
//The following pins are reserved for module use, cannot be used by customers, and cannot be modified
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
#define QUEC_PIN_DNAME_AP_JTAG_TRST      200
#define QUEC_PIN_DNAME_AP_JTAG_TDI       201
#define QUEC_PIN_DNAME_AP_JTAG_TDO       202
#define QUEC_PIN_DNAME_KEYOUT_4          203
#define QUEC_PIN_DNAME_KEYOUT_5          204
#endif
#ifdef CONFIG_GPIO_USED_FOR_VBAT_RF_SWITCH
#define QUEC_PIN_DNAME_AP_JTAG_TMS       205
#endif
/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_GPIO_21

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_KEYOUT_5
#else
#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_SDMMC1_DATA_3
#endif
/***********    EG912U End    ***********/

#endif
/************ End of Pin Default Name ************/


/************    Multiple Function Name    ************/
/************    use Quec Function Name    ************///add location:name for applicaiton
#define QUEC_PIN_LCD_SPI_SIO       QUEC_PIN_DNAME_SPI_LCD_SIO
#define QUEC_PIN_LCD_SPI_SDC       QUEC_PIN_DNAME_SPI_LCD_SDC
#define QUEC_PIN_LCD_SPI_CLK       QUEC_PIN_DNAME_SPI_LCD_CLK
#define QUEC_PIN_LCD_SPI_CS        QUEC_PIN_DNAME_SPI_LCD_CS
#define QUEC_PIN_LCD_SPI_SEL       QUEC_PIN_DNAME_SPI_LCD_SELECT
#define QUEC_PIN_LCD_FMARK         QUEC_PIN_DNAME_LCD_FMARK

//codec audio
#define QUEC_PIN_PCM_CLK           QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_PCM_SYNC          QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_PCM_DIN           QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_PCM_DOUT          QUEC_PIN_DNAME_GPIO_3

#define QUEC_PIN_CAM_I2C_SCL       QUEC_PIN_DNAME_I2C_M1_SCL
#define QUEC_PIN_CAM_I2C_SDA       QUEC_PIN_DNAME_I2C_M1_SDA
#define QUEC_PIN_CAM_RSTL          QUEC_PIN_DNAME_CAMERA_RST_L
#define QUEC_PIN_CAM_PWDN          QUEC_PIN_DNAME_CAMERA_PWDN
#define QUEC_PIN_CAM_REFCLK        QUEC_PIN_DNAME_CAMERA_REF_CLK
#define QUEC_PIN_CAM_SPI_DATA0     QUEC_PIN_DNAME_SPI_CAMERA_SI_0
#define QUEC_PIN_CAM_SPI_DATA1     QUEC_PIN_DNAME_SPI_CAMERA_SI_1
#define QUEC_PIN_CAM_SPI_SCK       QUEC_PIN_DNAME_SPI_CAMERA_SCK

#define QUEC_PIN_MIPI_LCD_DSI_CKP       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_MIPI_LCD_DSI_CKN       QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_MIPI_LCD_DSI_D0P       QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_MIPI_LCD_DSI_D0N       QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_MIPI_LCD_DSI_D1P       QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_MIPI_LCD_DSI_D1N       QUEC_PIN_DNAME_GPIO_2

//added by ryan.yi 2021/04/07
#if (defined CONFIG_QL_PROJECT_DEF_EC200U)
#define QL_EC200U_WITH_GNSS        1        //1-with GNSS,0-without GNSS
#if QL_EC200U_WITH_GNSS
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_7
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_AP_JTAG_TCK
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_AP_JTAG_TRST
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_AP_JTAG_TMS
#define QUEC_PIN_SPI1_FUNC         0x02
#else
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01
#endif

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x02

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EC600U)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x02

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EG700U)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01
//EG700U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG700U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI2_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_FUNC         0x00
/*
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x02
*/

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EG500U)
//EG500U只支持SPI2，定义SPI1只是为了统一，可以编译通过
//EG500U only support SPI2，definition of SPI1 is just for unification, and can be compiled passed
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI1_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_FUNC         0x00

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x02

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01

#elif (defined CONFIG_QL_PROJECT_DEF_EG915U)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01
//EG915U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG915U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI2_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_FUNC         0x00

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EG070U)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01
//EG070U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG070U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI2_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_FUNC         0x00

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EC020U)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x02

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EG912U)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_GPIO_9
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_GPIO_10
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_GPIO_11
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_GPIO_12
#define QUEC_PIN_SPI1_FUNC         0x01
//EG912U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG912U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI2_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_FUNC         0x00

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_DNAME_GPIO_4
#define QUEC_PIN_SPI6_SIO3_PIN     QUEC_PIN_DNAME_GPIO_5
#define QUEC_PIN_SPI6_FUNC         0x01

#else
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI1_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_FUNC         0x00

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI2_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI2_FUNC         0x00

#define QUEC_PIN_SPI6_CLK_PIN      QUEC_PIN_NONE
#define QUEC_PIN_SPI6_CS_PIN       QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO0_PIN     QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO1_PIN     QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO2_PIN     QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO3_PIN     0x00
#endif


/************    for standard     ************/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG

#define QUEC_PIN_NET_STATUS        QUEC_PIN_DNAME_GPIO_13           //for Standard:net status lamp

#ifdef CONFIG_QL_PROJECT_DEF_EC200U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SPI_CAMERA_SCK

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_CAMERA_PWDN       //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_SPI_CAMERA_SI_1

#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
#define QUEC_PIN_SLEEP_IND        QUEC_PIN_NONE
#define QUEC_PIN_WAKEUP_IN        QUEC_PIN_NONE
#define QUEC_PIN_AP_READY         QUEC_PIN_NONE
#define QUEC_PIN_W_DISABLE        QUEC_PIN_NONE
#else
#define QUEC_PIN_SLEEP_IND        QUEC_PIN_DNAME_GPIO_8
#define QUEC_PIN_WAKEUP_IN        QUEC_PIN_DNAME_SIM_2_CLK
#define QUEC_PIN_AP_READY         QUEC_PIN_DNAME_SIM_2_DIO
#define QUEC_PIN_W_DISABLE        QUEC_PIN_DNAME_SIM_2_RST
#endif
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_SPI_CAMERA_SCK

#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE
#define QUEC_GPIO_WAKEUP_IN        QUEC_PIN_NONE
#define QUEC_GPIO_AP_READY         QUEC_PIN_NONE
#define QUEC_GPIO_W_DISABLE        QUEC_PIN_NONE
#else
#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_GPIO_8
#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SIM_2_CLK
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SIM_2_DIO
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SIM_2_RST
#endif

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_SPI_CAMERA_SI_1

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_CAMERA_PWDN
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SPI_CAMERA_SI_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_CAMERA_REF_CLK

/***********    EC200U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EC600U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_NONE                    //EC600U Standard Ver. haven't SD_DET

#ifdef __QUEC_OEM_VER_LD__
#define QUEC_PIN_MAIN_RI           QUEC_PIN_NONE                    //LD Ver. haven't MAIN_RI
#define QUEC_PIN_NET_MODE          QUEC_PIN_NONE                    //LD Ver. haven't NET_MODE
#define QUEC_PIN_SLEEP_IND         QUEC_PIN_NONE
#define QUEC_PIN_W_DISABLE         QUEC_PIN_NONE
#else
#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_SDMMC1_DATA_1     //for Standard:advanced initialization
#define QUEC_PIN_WAKEUP_IN         QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_AP_READY          QUEC_PIN_DNAME_SDMMC1_DATA_3
 #ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
#define QUEC_PIN_NET_MODE          QUEC_PIN_NONE
#define QUEC_PIN_SLEEP_IND         QUEC_PIN_NONE
#define QUEC_PIN_W_DISABLE         QUEC_PIN_NONE
 #else /* public */
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_SIM_2_DIO
#define QUEC_PIN_SLEEP_IND         QUEC_PIN_DNAME_SIM_2_CLK
#define QUEC_PIN_W_DISABLE         QUEC_PIN_DNAME_SIM_2_RST
 #endif
#endif

/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_PIN_NONE                    //EC600U Standard Ver. haven't SD_DET

#ifdef __QUEC_OEM_VER_LD__
#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE                    //LD Ver. haven't SLEEP_IND
#define QUEC_GPIO_NET_MODE         QUEC_PIN_NONE                    //LD Ver. haven't NET_MODE
#define QUEC_GPIO_MAIN_DTR         QUEC_PIN_NONE                    //LD Ver. haven't MAIN_DTR
#define QUEC_GPIO_MAIN_RI          QUEC_PIN_NONE                    //LD Ver. haven't MAIN_RI
#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_GPIO_5           //LD Ver. is GPIO_5
#define QUEC_GPIO_AP_READY         QUEC_PIN_NONE                    //LD Ver. haven't AP_READY
#define QUEC_GPIO_W_DISABLE        QUEC_PIN_NONE                    //LD Ver. haven't W_DISABLE
#else /* public */
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SDMMC1_DATA_0
#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_SDMMC1_DATA_1
#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SDMMC1_DATA_2
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SDMMC1_DATA_3
 #ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE
#define QUEC_GPIO_NET_MODE         QUEC_PIN_NONE
#define QUEC_GPIO_W_DISABLE        QUEC_PIN_NONE
 #else /* public */
#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_SIM_2_CLK
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_SIM_2_DIO
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SIM_2_RST
 #endif
#endif

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13

#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_SDMMC1_CMD

/***********    EC600U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG700U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SPI_LCD_CS

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_GPIO_15           //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_SPI_LCD_SELECT

/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_SPI_LCD_CS

#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_LCD_FMARK
#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_SPI_LCD_SELECT

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_GPIO_15
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_GPIO_8
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_GPIO_14

#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SPI_LCD_CLK
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SPI_LCD_CS
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SPI_LCD_SIO

/***********    EG700U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG500U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_GPIO_3

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_GPIO_1            //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_SIM_2_DIO

/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_GPIO_3

#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE                    //EG500U Standard Ver. haven't SLEEP_IND
#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_SIM_2_DIO

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_GPIO_1
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_GPIO_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_GPIO_2
#define QUEC_GPIO_MAIN_DSR         QUEC_GPIO_DNAME_GPIO_3

#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SIM_2_CLK
#define QUEC_GPIO_AP_READY         QUEC_PIN_NONE                    //EG500U Standard Ver. haven't AP_READY
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SIM_2_RST

/***********    EG500U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG915U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_NONE                    //EG915U Standard Ver. haven't SD_DET

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_SDMMC1_DATA_1     //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_NONE                    //EG915U Standard Ver. haven't NET_MODE

#define QUEC_PIN_FAST_POWEROFF     QUEC_PIN_DNAME_GPIO_10           //EG915U use pin num 25 for fast/poweroff
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_PIN_NONE                    //EG915U Standard Ver. haven't SD_DET

#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE                    //EG915U Standard Ver. haven't SLEEP_IND
#define QUEC_GPIO_PSM_IND          QUEC_GPIO_DNAME_GPIO_4
#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_PIN_NONE                    //EG915U Standard Ver. haven't NET_MODE

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_SDMMC1_DATA_1
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SDMMC1_DATA_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_SDMMC1_CMD

#define QUEC_GPIO_WAKEUP_IN        QUEC_PIN_NONE                    //EG915U Standard Ver. haven't WAKEUP_IN
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SDMMC1_DATA_3
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SDMMC1_DATA_2

#define QUEC_GPIO_FAST_POWEROFF    QUEC_GPIO_DNAME_GPIO_10          //EG915U Standard Ver. Only it has the fast/poweroff 

/***********    EG915U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG070U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SPI_LCD_CS

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_GPIO_15           //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_SPI_LCD_SELECT

/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_SPI_LCD_CS

#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_LCD_FMARK
#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_SPI_LCD_SELECT

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_GPIO_15
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_GPIO_8
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_GPIO_14

#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SPI_LCD_CLK
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SPI_LCD_CS
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SPI_LCD_SIO

/***********    EG070U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EC020U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_GPIO_15

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_SPI_CAMERA_SI_1       //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_CAMERA_PWDN

/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_GPIO_15

#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_SPI_CAMERA_SCK
#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_CAMERA_PWDN

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_SPI_CAMERA_SI_1
#define QUEC_GPIO_MAIN_DTR         QUEC_PIN_NONE                        //EC020U Standard Ver. haven't MAIN_DTR    //QUEC_GPIO_DNAME_GPIO_20
#define QUEC_GPIO_MAIN_DCD         QUEC_PIN_NONE                        //EC020U Standard Ver. haven't MAIN_DCD    //QUEC_GPIO_DNAME_GPIO_21

#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SIM_2_CLK
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SIM_2_DIO
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SIM_2_RST

/***********    EC020U End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG912U

/*************** Pin ****************/
#define QUEC_PIN_SD_DET            QUEC_PIN_NONE                    //EG912U Standard Ver. haven't SD_DET

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_SDMMC1_DATA_1     //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_NONE                    //EG912U Standard Ver. haven't NET_MODE

#define QUEC_PIN_FAST_POWEROFF     QUEC_PIN_DNAME_GPIO_10           //EG912U use pin num 25 for fast/poweroff
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_PIN_NONE                    //EG912U Standard Ver. haven't SD_DET

#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE                    //EG912U Standard Ver. haven't SLEEP_IND
#define QUEC_GPIO_PSM_IND          QUEC_GPIO_DNAME_GPIO_4
#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_13
#define QUEC_GPIO_NET_MODE         QUEC_PIN_NONE                    //EG912U Standard Ver. haven't NET_MODE

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_SDMMC1_DATA_1
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SDMMC1_DATA_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_SDMMC1_CMD

#define QUEC_GPIO_WAKEUP_IN        QUEC_PIN_NONE                    //EG912U Standard Ver. haven't WAKEUP_IN
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SDMMC1_DATA_3
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SDMMC1_DATA_2
/***********    EG912U End    ***********/

#endif

#endif/* CONFIG_QL_OPEN_EXPORT_PKG */


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_PIN_INDEX_H */


