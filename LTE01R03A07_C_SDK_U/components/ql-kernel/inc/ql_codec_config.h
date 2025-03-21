
/**  @file
  ql_codec_config.h

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
                
#ifndef _QL_CODEC_CONFIG_H_
#define _QL_CODEC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//CODEC ALC5616
#define RT5616_I2C_SLAVE_ADDR       0x1B
#define RT5616_VENDOR_ID	        0x10EC
#define RT5616_VENDOR_ID_REG        0XFE
//CODEC nau8810
#define NAU8810_I2C_SLAVE_ADDR      0x1A
#define NAU8810_VENDOR_ID	        0x1A
#define NAU8810_VENDOR_ID_REG       0X3F

//CODEC ALC5616E
#define RT5616E_I2C_SLAVE_ADDR      0x1B
#define RT5616E_VENDOR_ID	        0x10EC
#define RT5616E_VENDOR_ID_REG       0XFE


/*****************************************************************************************
* Function: RT5616_INIT_REG
*
* Description: the register below will be write to codec by your code, can be write when power on
* you can add your register below : {register_addr , value,  delay}
******************************************************************************************/
#define RT5616_INIT_REG				\
{									\
	{0x02,	0x0808,	0x00},			\
	{0x03,	0x0808,	0x00},			\
	{0x05,	0x8000, 0x00},			\
	{0x0d,  0x04C0,	0x00},			\
	{0x0F,	0x0808, 0x00},			\
	{0x19,	0xAFAF, 0x00},			\
	{0x1C,	0x3F3F, 0x00},			\
	{0x1E,	0x0000, 0x00},			\
	{0x27,	0x3820, 0x00},			\
	{0x29,	0x8080, 0x00},			\
	{0x2A,	0x1250, 0x00},			\
	{0x3B,	0x0000, 0x00},			\
	{0x3C,	0x006B, 0x00},			\
	{0x3D,	0x2000, 0x00},			\
	{0x3E,	0x006B, 0x00},			\
	{0x45,	0x4000, 0x00},			\
	{0x4D,	0x0000, 0x00},			\
	{0x4E,	0x0000, 0x00},			\
	{0x4F,	0x0278, 0x00},			\
	{0x50,	0x0000, 0x00},			\
	{0x51,	0x0000, 0x00},			\
	{0x52,	0x0278, 0x00},			\
	{0x53,	0xC800, 0x00},			\
	{0x61,	0x9806, 0x00},			\
	{0x62,	0x8800,	0x00},    		\
	{0x63,	0xF8FE,	0x00},			\
	{0x64,  0x6A00, 0x00},    		\
	{0x65,	0xCC00,	0x66},			\
	{0X66,	0x3F00,	0x00},			\
	{0x6A,	0x003D, 0x00},			\
	{0x6C,	0x3600, 0x00},			\
	{0x70,  0x0000, 0x00},			\
	{0x74,	0x0C00, 0x00},			\
	{0x8E,	0x0019, 0x00},			\
	{0x8F,	0x3100, 0x00},			\
	{0x93,	0x2C00, 0x00},			\
	{0x94,	0x0200, 0x00},			\
	{0xB0,	0x2080, 0x00},			\
	{0xB1,	0x0000, 0x00},			\
	{0xFA,	0x0011, 0x00},			\
}

/*****************************************************************************************
* Function: RT5616_NV_INIT_REG
*
* Description: the register below will be write to codec by dsp when start play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 100 
******************************************************************************************/
#define RT5616_NV_INIT_REG			\
{									\
}

/*****************************************************************************************
* Function: RT5616_NV_CLOSE_REG
*
* Description: the register below will be write to codec by dsp when stop play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 50 
******************************************************************************************/
#define RT5616_NV_CLOSE_REG 		\
{									\
}

/*****************************************************************************************
* Function: RT5616_NV_SAMPLE_REG
*
* Description: the register below will be write to codec by dsp when stop play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 50 
******************************************************************************************/
#define RT5616_NV_SAMPLE_8K_REG \
{								\
	{0x73,	0x1100,	  0x00},	\
	{0x80,	0x4000,	  0x00},  	\
	{0x81,	0x249A,	  0x00},	\
	{0x82,  0xF000,   0x00},  	\
}

#define RT5616_NV_SAMPLE_12K_REG \
{							     \
	{0x73,	0x1100,   0x00},	\
	{0x80,	0x4000,	  0x00},  	\
	{0x81,	0x148C,	  0x00},	\
	{0x82,  0xB000,   0x00},  	\
}

#define RT5616_NV_SAMPLE_16K_REG \
{								 \
	{0x73,	0x1100,   0x00},	 \
	{0x80,	0x4000,	  0x00},  	 \
	{0x81,	0x248C,	  0x00},	 \
	{0x82,  0xF000,   0x00},  	 \
}

#define RT5616_NV_SAMPLE_22_05K_REG \
{									\
}

#define RT5616_NV_SAMPLE_24K_REG   \
{								   \
}

#define RT5616_NV_SAMPLE_32K_REG \
{								 \
	{0x73,	0x1100,   0x00},	 \
	{0x80,	0x4000,	  0x00},  	 \
	{0x81,	0x2485,	  0x00},	 \
	{0x82,  0xF000,   0x00},  	 \
}

#define RT5616_NV_SAMPLE_44_1K_REG \
{								   \
	{0x73,	0x1100,   0x00},	 \
	{0x80,	0x4000,	  0x00},  	 \
	{0x81,	0x1C82,	  0x00},	 \
	{0x82,  0xF000,   0x00},  	 \
}

#define RT5616_NV_SAMPLE_48K_REG   \
{								   \
	{0x73,	0x1100,   0x00},	\
	{0x80,	0x4000,	  0x00},  	\
	{0x81,	0x1903,	  0x00},	\
	{0x82,  0x9000,   0x00},  	\
}



/**************************************************************************************************
* Function: RT5616_NV_INPUT_REG
*
* Description: the register below will be write to codec by dsp when choose following mic to record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 20 
***************************************************************************************************/
#define RT5616_INPUT_MAIN_MIC_REG  \
{								   \
}

#define RT5616_INPUT_AUX_MIC_REG   \
{								   \
}

#define RT5616_INPUT_DUAL_MIC_REG  \
{								   \
}

#define RT5616_INPUT_HP_MIC_L_REG  \
{								   \
}

#define RT5616_INPUT_HP_MIC_R_REG  \
{								   \
}


/*****************************************************************************************************
* Function: RT5616_NV_OUTPUT_REG
*
* Description: the register below will be write to codec by dsp when use following output type to play
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 20 
******************************************************************************************************/
#define RT5616_OUTPUT_RECEIVER_REG  \
{								    \
}

#define RT5616_OUTPUT_HEADPHONE_REG \
{								    \
}

#define RT5616_OUTPUT_SPEAKER_REG   \
{								    \
}

//NOT support now
#define RT5616_OUTPUT_HEADPHONE_SPEAKER_REG  \
{								    		 \
}


/*****************************************************************************************
* Function: NAU8810_INIT_REG
*
* Description: the register below will be write to codec by your code, can be write when power on
* you can add your register below : {register_addr , value,  delay}
******************************************************************************************/

#define NAU8810_INIT_REG				\
{									\
	{0x01,	0x013D,	0x00},			\
	{0x02,	0x0015,	0x00},			\
	{0x03,	0x00FD,	0x00},			\
	{0x04,	0x0012, 0x00},			\
	{0x06,	0x000D, 0x00},			\
	{0x07,	0x000A, 0x00},			\
	{0x0A,	0x0008, 0x00},			\
	{0x0E,	0x0119, 0x00},			\
	{0x2C,	0x0003, 0x00},			\
	{0x2D,	0x0024, 0x00},			\
	{0x32,	0x0001,	0x01},			\
	{0x38,  0x0001, 0x00},  		\
	{0x2F,  0x0000, 0x00},  		\
	{0x3C,  0x0018, 0x00},  		\
}

/*****************************************************************************************
* Function: NAU8810_NV_INIT_REG
*
* Description: the register below will be write to codec by dsp when start play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 100
******************************************************************************************/
#define NAU8810_NV_INIT_REG		\
{								\
}

/*****************************************************************************************
* Function: NAU8810_NV_CLOSE_REG
*
* Description: the register below will be write to codec by dsp when stop play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 50 
******************************************************************************************/
#define NAU8810_NV_CLOSE_REG \
{						    \
}

/*****************************************************************************************
* Function: NAU8810_NV_SAMPLE_REG
*
* Description: the register below will be write to codec by dsp when stop play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 50 
******************************************************************************************/
#define NAU8810_NV_SAMPLE_8K_REG \
{								\
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x01ED,	  0x00},  	\
	{0x7,	0x000B,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}
#define NAU8810_NV_SAMPLE_11_025K_REG \
{									\
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x01CD,	  0x00},  	\
	{0x7,	0x0009,	  0x00},  	\
	{0x24,	0x0016,	  0x00},	\
	{0x25,  0x003C,   0x00},  	\
	{0x26,  0x0145,   0x00},  	\
	{0x27,  0x01D4,   0x00},  	\
}


#define NAU8810_NV_SAMPLE_12K_REG \
{							     \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x01CD,	  0x00},  	\
	{0x7,	0x0009,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}

#define NAU8810_NV_SAMPLE_16K_REG \
{								 \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x01AD,	  0x00},  	\
	{0x7,	0x0007,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}

#define NAU8810_NV_SAMPLE_22_05K_REG \
{									\
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x018D,	  0x00},  	\
	{0x7,	0x0007,	  0x00},  	\
	{0x24,	0x0016,	  0x00},	\
	{0x25,  0x003C,   0x00},  	\
	{0x26,  0x0145,   0x00},  	\
	{0x27,  0x01D4,   0x00},  	\
}

#define NAU8810_NV_SAMPLE_24K_REG   \
{								   \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x018D,	  0x00},  	\
	{0x7,	0x0007,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}

#define NAU8810_NV_SAMPLE_32K_REG \
{								 \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x016D,	  0x00},  	\
	{0x7,	0x0003,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}

#define NAU8810_NV_SAMPLE_44_1K_REG \
{								   \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x014D,	  0x00},  	\
	{0x7,	0x0001,	  0x00},  	\
	{0x24,	0x0016,	  0x00},	\
	{0x25,  0x003C,   0x00},  	\
	{0x26,  0x0145,   0x00},  	\
	{0x27,  0x01D4,   0x00},  	\
}

#define NAU8810_NV_SAMPLE_48K_REG   \
{								   \
									 \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x014D,	  0x00},  	\
	{0x7,	0x0001,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}
#define NAU8810_NV_SAMPLE_64K_REG   \
{								   \
									 \
	{0x4,	0x0012,   0x00},	\
	{0x6,	0x012D,	  0x00},  	\
	{0x7,	0x0001,	  0x00},  	\
	{0x24,	0x0017,	  0x00},	\
	{0x25,  0x0023,   0x00},  	\
	{0x26,  0x01EA,   0x00},  	\
	{0x27,  0x0126,   0x00},  	\
}




/**************************************************************************************************
* Function: NAU8810_NV_INPUT_REG
*
* Description: the register below will be write to codec by dsp when choose following mic to record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 20 
***************************************************************************************************/
#define NAU8810_INPUT_MAIN_MIC_REG  \
{								   \
}

#define NAU8810_INPUT_AUX_MIC_REG   \
{								   \
}

#define NAU8810_INPUT_DUAL_MIC_REG  \
{								   \
}

#define NAU8810_INPUT_HP_MIC_L_REG  \
{								   \
}

#define NAU8810_INPUT_HP_MIC_R_REG  \
{								   \
}


/*****************************************************************************************************
* Function: NAU8810_NV_OUTPUT_REG
*
* Description: the register below will be write to codec by dsp when use following output type to play
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 20 
******************************************************************************************************/
#define NAU8810_OUTPUT_RECEIVER_REG  \
{								    \
}

#define NAU8810_OUTPUT_HEADPHONE_REG \
{								    \
}

#define NAU8810_OUTPUT_SPEAKER_REG   \
{								    \
}

//NOT support now
#define NAU8810_OUTPUT_HEADPHONE_SPEAKER_REG  \
{								    		 \
}

/*****************************************************************************************
* Function: RT5616E_INIT_REG
*
* Description: the register below will be write to codec by your code, can be write when power on
* you can add your register below : {register_addr , value,  delay}
******************************************************************************************/
#define RT5616E_INIT_REG				\
{									    \
    {0x0000,    0x0000, 0x10},          \
	{0x008F,	0x1140,	0x00},			\
	{0x0063,	0xa8d6, 0x10},			\
	{0x0063,	0xe8fe, 0x00},			\
	{0x00D6,    0x0011,	0x00},			\
	{0x0208,	0x0200, 0x00},			\
	{0x3000,	0x9F00, 0x10},			\
	{0x00D6,	0x0400, 0x10},			\
	{0x0114,	0x280A, 0x00},			\
	{0x00FA,	0x0091, 0x00},			\
	{0x0061,	0x1800, 0x00},			\
	{0x0064,	0x0004, 0x00},			\
	{0x0066,	0x0C00, 0x00},			\
	{0x0067,	0xC300, 0x00},			\
	{0x008E,	0x0009, 0x00},			\
	{0x3100,	0x000C, 0x00},			\
	{0x002a,	0x0202, 0x00},			\
	{0x0045,	0xC000, 0x00},			\
	{0x013D,	0xbe00, 0x00},			\
	{0x0081,	0x248c,	0x00},	        \
	{0x0082,    0xf400, 0x00},  	    \
	{0x2200,	0x8018, 0x00},			\
	{0x0200,	0x00F1, 0x00},			\
	{0x0201,	0x0001, 0x00},			\
	{0x0203,	0x0011, 0x00},			\
	{0x0204,	0x0011,	0x00},    		\
	{0x0206,	0x0003,	0x00},			\
	{0x0207,    0x0311, 0x00},    		\
	{0x0208,	0x0323,	0x10},			\
	{0X0053,	0x3000,	0x00},			\
	{0x0005,	0x8000, 0x00},			\
	{0x0003,	0x4848, 0x00},			\
	{0x0e02,    0x0090, 0x00},			\
	{0x003C,	0x006F, 0x00},			\
	{0x0061,	0x1806, 0x00},			\
	{0x0064,	0x8834, 0x00},			\
	{0x0065,	0x0800, 0x66},			\
	{0x0093,	0x2020, 0x00},			\
	{0x000d,	0x1080, 0x00},			\
	{0x0027,	0x2020, 0x00},			\
	{0x0E02,	0x0000, 0x00},			\
	{0x0002,	0x8888, 0x10},			\
	{0x0002,	0x0808, 0x00},			\
	{0x0063,	0xf8fe, 0x00},			\
	{0x0115,	0xAA88, 0x00},			\
	{0x0300,    0x0080, 0x10},  	    \
	{0x0300,    0x0040, 0x10},  	    \
	{0x0300,    0x0080, 0x10},  	    \
	{0x0300,    0x0040, 0x10},  	    \
	{0x2200,    0x8010, 0x00},  	    \
}


/*****************************************************************************************
* Function: RT5616E_NV_INIT_REG
*
* Description: the register below will be write to codec by dsp when start play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 100 
******************************************************************************************/
#define RT5616E_NV_INIT_REG		\
{								\
}

/*****************************************************************************************
* Function: RT5616E_NV_CLOSE_REG
*
* Description: the register below will be write to codec by dsp when stop play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 50 
******************************************************************************************/
#define RT5616E_NV_CLOSE_REG \
{						    \
}

/*****************************************************************************************
* Function: RT5616E_NV_SAMPLE_REG
*
* Description: the register below will be write to codec by dsp when stop play or record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 50 
******************************************************************************************/
#define RT5616E_NV_SAMPLE_8K_REG \
{								\
	{0x0081,	0x0C1C,	0x00},	        \
	{0x0082,    0x9400, 0x00},  	    \
}

#define RT5616E_NV_SAMPLE_12K_REG \
{							     \
	{0x0081,	0x129C,	0x00},	        \
	{0x0082,    0x9400, 0x00},  	    \
}

#define RT5616E_NV_SAMPLE_16K_REG \
{								 \
	{0x0081,	0x1914,	0x00},	        \
	{0x0082,    0xd400, 0x00},  	    \
}

#define RT5616E_NV_SAMPLE_22_05K_REG \
{									\
}

#define RT5616E_NV_SAMPLE_24K_REG   \
{								   \
}

#define RT5616E_NV_SAMPLE_32K_REG \
{								 \
	{0x0081,	0x248c,	0x00},	        \
	{0x0082,    0xf400, 0x00},  	    \
}

#define RT5616E_NV_SAMPLE_44_1K_REG \
{								   \
}

#define RT5616E_NV_SAMPLE_48K_REG   \
{								   \
}



/**************************************************************************************************
* Function: RT5616E_NV_INPUT_REG
*
* Description: the register below will be write to codec by dsp when choose following mic to record
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 20 
***************************************************************************************************/
#define RT5616E_INPUT_MAIN_MIC_REG  \
{								   \
}

#define RT5616E_INPUT_AUX_MIC_REG   \
{								   \
}

#define RT5616E_INPUT_DUAL_MIC_REG  \
{								   \
}

#define RT5616E_INPUT_HP_MIC_L_REG  \
{								   \
}

#define RT5616E_INPUT_HP_MIC_R_REG  \
{								   \
}


/*****************************************************************************************************
* Function: RT5616E_NV_OUTPUT_REG
*
* Description: the register below will be write to codec by dsp when use following output type to play
* you can add your register below : {register_addr , value,  delay}
* Attention: The maximum number can not exceed 20 
******************************************************************************************************/
#define RT5616E_OUTPUT_RECEIVER_REG  \
{								    \
}

#define RT5616E_OUTPUT_HEADPHONE_REG \
{								    \
}

#define RT5616E_OUTPUT_SPEAKER_REG   \
{								    \
}

//NOT support now
#define RT5616E_OUTPUT_HEADPHONE_SPEAKER_REG  \
{								    		 \
}


#ifdef __cplusplus
} /*"C" */
#endif

#endif

