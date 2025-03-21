/**  @file
  ql_cnktd_spi.h

  @brief
  This file is used to define cnktd spi for different Quectel Project.

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

=================================================================*/


#ifndef QL_CNKTD_SPI_H
#define QL_CNKTD_SPI_H

#include "ql_cnktd.h"
#include "ql_api_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TXM_QAPI_BUSES_SPI_BASE  ((int)QL_COMPONENT_APP_START << 16 | 1000)
#define TXM_QAPI_SPI_OPEN        (TXM_QAPI_BUSES_SPI_BASE)
#define TXM_QAPI_SPI_FULL_DUPLEX (TXM_QAPI_BUSES_SPI_BASE + 1)
#define TXM_QAPI_SPI_POWER_ON    (TXM_QAPI_BUSES_SPI_BASE + 2)
#define TXM_QAPI_SPI_POWER_OFF   (TXM_QAPI_BUSES_SPI_BASE + 3)


/*========================================================================
 *  Variable Definition
 *========================================================================*/

/**
* SPI instance enumeration.
*
* This enumeration lists the possible SPI instance indicating which HW SPI master 
* is to be used for the current SPI transaction.
*/

typedef enum
{
    QAPI_SPIM_INSTANCE_1_E = 1, /**< SPIM instance 1. */
    QAPI_SPIM_INSTANCE_2_E,     /**< SPIM instance 2. */
    QAPI_SPIM_INSTANCE_3_E,     /**< SPIM instance 3. */
    QAPI_SPIM_INSTANCE_4_E,     /**< SPIM instance 4. */
    QAPI_SPIM_INSTANCE_5_E,     /**< SPIM instance 5. */
    QAPI_SPIM_INSTANCE_6_E,     /**< SPIM instance 6. */
    QAPI_SPIM_INSTANCE_7_E,     /**< SPIM instance 7. */
    QAPI_SPIM_INSTANCE_8_E,     /**< SPIM instance 8. */
    QAPI_SPIM_INSTANCE_9_E,     /**< SPIM instance 9. */
    QAPI_SPIM_INSTANCE_10_E,    /**< SPIM instance 10. */
    QAPI_SPIM_INSTANCE_11_E,    /**< SPIM instance 11. */
    QAPI_SPIM_INSTANCE_12_E,    /**< SPIM instance 12. */
    QAPI_SPIM_INSTANCE_13_E,    /**< SPIM instance 13. */
    QAPI_SPIM_INSTANCE_14_E,    /**< SPIM instance 14. */
    QAPI_SPIM_INSTANCE_15_E,    /**< SPIM instance 15. */
    QAPI_SPIM_INSTANCE_16_E,    /**< SPIM instance 16. */
    QAPI_SPIM_INSTANCE_17_E,    /**< SPIM instance 17. */
    QAPI_SPIM_INSTANCE_18_E,    /**< SPIM instance 18. */
    QAPI_SPIM_INSTANCE_19_E,    /**< SPIM instance 19. */
    QAPI_SPIM_INSTANCE_20_E,    /**< SPIM instance 20. */
    QAPI_SPIM_INSTANCE_21_E,    /**< SPIM instance 21. */
    QAPI_SPIM_INSTANCE_22_E,    /**< SPIM instance 22. */
    QAPI_SPIM_INSTANCE_23_E,    /**< SPIM instance 23. */
    QAPI_SPIM_INSTANCE_24_E,    /**< SPIM instance 24. */
    QAPI_SPIM_INSTANCE_MAX_E,
} qapi_SPIM_Instance_t;

/**
* SPI phase type.
*
* This type defines the clock phase that the client can set in the 
* SPI configuration.
*/
typedef enum 
{
   QAPI_SPIM_MODE_0_E, /**< CPOL = 0, CPHA = 0. */
   QAPI_SPIM_MODE_1_E, /**< CPOL = 0, CPHA = 1. */
   QAPI_SPIM_MODE_2_E,  /**< CPOL = 1, CPHA = 0. */
   QAPI_SPIM_MODE_3_E,  /**< CPOL = 1, CPHA = 1. */
   QAPI_SPIM_MODE_INVALID_E = 0x7FFFFFFF
}qapi_SPIM_Shift_Mode_t;

/** SPI chip select ppolarity type.
*/
typedef enum 
{
   QAPI_SPIM_CS_ACTIVE_LOW_E,   /**< During Idle state, the CS line is held low. */
   QAPI_SPIM_CS_ACTIVE_HIGH_E,  /**< During Idle state, the CS line is held high. */
   QAPI_SPIM_CS_ACTIVE_INVALID_E = 0x7FFFFFFF
}qapi_SPIM_CS_Polarity_t;

/*Note - Endian ness - it is unclear  whether we can 
 * support this in all (FIFO/GSI) modes ! */

/** Order in which bytes from Tx/Rx buffer words are put on the bus.
 */
typedef enum
{
    SPI_NATIVE = 0,         /**< Native. */
    SPI_LITTLE_ENDIAN = 0,  /**< Little Endian. */
    SPI_BIG_ENDIAN          /**< Big Endian (network). */
}qapi_SPIM_Byte_Order_t;

/**
* SPI chip select assertion type.
*
* This type defines how the chip select line is configured between N word cycles.
*/
typedef enum 
{
   QAPI_SPIM_CS_DEASSERT_E,/**< CS is deasserted after transferring data for N clock cycles. */
   QAPI_SPIM_CS_KEEP_ASSERTED_E,/**< CS is asserted as long as the core is in the Run state. */
   QAPI_SPIM_CS_MODE_INVALID_E = 0x7FFFFFFF
}qapi_SPIM_CS_Mode_t;

/**
* SPI master configuration.
*
* The SPI master configuration is the collection of settings specified for each SPI
* transfer call to select the various possible SPI transfer parameters.
*/
typedef struct
{
   /* Add note to timing diagram here.*/
   qapi_SPIM_Shift_Mode_t SPIM_Mode;
   /**< SPIM mode type to be used for the SPI core. */
   qapi_SPIM_CS_Polarity_t SPIM_CS_Polarity;
   /**< CS polarity type to be used for the SPI core. */
   qapi_SPIM_Byte_Order_t SPIM_endianness;
   /** Endian-ness type used for the SPI transfer. */
   uint8_t SPIM_Bits_Per_Word;
   /**< SPI bits per word; any value from 3 to 31. */
   uint8_t SPIM_Slave_Index;
   /**< Slave index, beginning at 0 if mulitple SPI devices are connected to the same master. 
   At most 7 slaves are allowed. If an invalid number (7 or higher) is set, the CS 
   signal will not be used. */
   uint32_t Clk_Freq_Hz;
   /**< Host sets the SPI clock frequency closest to the requested frequency. */
#if 1  //QuectelModifyFlag
   uint32_t CS_Clk_Delay_Cycles;
#else
   uint8_t CS_Clk_Delay_Cycles;
#endif
   /**< Number of clock cycles to wait after asserting CS before starting transfer. */
#if 1  //QuectelModifyFlag
   uint32_t Inter_Word_Delay_Cycles;
#else
   uint8_t Inter_Word_Delay_Cycles;
#endif
   /**< Number of clock cycles to wait between SPI words. */
   qapi_SPIM_CS_Mode_t SPIM_CS_Mode;
   /**< CS mode to be used for the SPI core. */
   qbool_t loopback_Mode;
   /**< Normally 0. If set, the SPI controller will enable Loopback mode; used primarily for testing. */
} qapi_SPIM_Config_t;

/**
* SPI transfer type.
*
* This type specifies the address and length of the buffer for an SPI transaction.
*/
typedef struct
{
   uint8_t *tx_buf;
   /**< Buffer address for transmitting data. */
   uint8_t *rx_buf;
   /**< Buffer address for receiving data. */
   uint32_t len; 
   /**< Size in bytes. No alignment requirements; the arbitrary length data can be transferred. */
}qapi_SPIM_Descriptor_t;

/**
* SPI callback function type.
*
* This type is used by the client to register its callback notification function. The 
* callback_Ctxt is the context object that will be passed untouched by the SPI Master
* driver to help the client identify which transfer completion instance is being signaled.
*/
typedef void (*qapi_SPIM_Callback_Fn_t) (uint32_t status, void *callback_Ctxt);

/*========================================================================
 *  function Definition
 *========================================================================*/

int spi_qapi_handler(int request_ID, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8);

/**
*  Initializes the SPI Master.
*
*  This function initializes internal data structures along with associated
*  static data. In any operating mode, this function should be called before
*  calling any other SPI master API.
*
*  @param[in]  instance    SPI instance specified by #qapi_SPIM_Instance_t.
*  @param[out] spi_Handle  Pointer to a location in which to store the driver handle.
*
*  @return
*  - QAPI_OK                           -- Module initialized successfully.
*  - QAPI_SPIM_ERROR_INVALID_PARAM     -- Invalid instance or handle parameter.
*  - QAPI_SPIM_ERROR_OPEN_FAILURE      -- Failed to Open SPIM.
*/
qapi_Status_t qapi_SPIM_Open (qapi_SPIM_Instance_t instance, void **spi_Handle);


/**
*  Enables the SPI Hardware resources for an SPI transaction.
*
*  This function enables all resources required for a successful SPI transaction. This 
*  includes clocks, power resources and pin multiplex functions. This function should 
*  be called before a transfer or a batch of SPI transfers.
* 
*  @param[in] spi_Handle  Driver handle returned by qapi_SPIM_Open().
*
*  @return
*  - QAPI_OK                             -- SPI master enabled successfully.
*  - QAPI_SPIM_ERROR_INVALID_PARAM       -- Invalid handle parameter.
*  - QAPI_SPIM_ERROR_POWER_ON_FAILURE    -- Failure in Power On.
*
*  @dependencies
*  - Before calling this API qapi_SPIM_Open() API must be called.
*/
qapi_Status_t qapi_SPIM_Power_On (void *spi_Handle);

/**
*  Disables the SPI Hardware resources for an SPI transaction.
*
*  This function turns off all resources used by the SPI master. This includes clocks, 
*  power resources, and GPIOs. This function should be called to put the SPI master
*  in its lowest possible power state.
* 
*  @param[in] spi_Handle  Driver handle returned by qapi_SPIM_Open().
*
*  @return
*  - QAPI_OK                             -- SPI master disabled successfully.
*  - QAPI_SPIM_ERROR_INVALID_PARAM       -- Invalid handle parameter.
*  - QAPI_SPIM_ERROR_POWER_OFF_FAILURE   -- Failure in Power Off.
*
*  @dependencies
*  - Before calling this API qapi_SPIM_Open() and qapi_SPIM_Power_On() API must be called.
*/
qapi_Status_t qapi_SPIM_Power_Off (void *spi_Handle);

/**
*  Performs a data transfer over the SPI bus.
*
*  This function performs an asynchronous transfer over the SPI bus. Transfers can be 
*  one-directional or bi-directional. A callback is generated upon transfer completion.
* 
*  @param[in] spi_Handle  Driver handle returned by qapi_SPIM_Open().
*  @param[in] config      Pointer to the SPI configuration structure described by
                          #qapi_SPIM_Config_t.
*  @param[in] desc        Pointer to the structure described by
                          #qapi_SPIM_Descriptor_t. The descriptor can have NULL Tx OR
                          Rx buffers if a half duplex transfer is selected.
*  @param[in] num_Descriptors  Number of descriptor pointers the client wants to process.
*  @param[in] c_Fn        Callback function to be invoked when the SPI transfer 
                          completes succesfully or with an error.
*  @param[in] c_Ctxt      Pointer to a client object that will be returned as an
                          argument to c_Fn.
*  @param[in] get_timestamp Boolean variable to indicate whether a tranasaction timestamp needs to be provided;
						  this is not supported for the QUPv2 version.
*  @return
*  - QAPI_OK                             -- SPI master was enabled successfully.
*  - QAPI_SPIM_ERROR_INVALID_PARAM       -- One or more invalid parameters.
*  - QAPI_SPIM_ERROR_TRANSFER_CONFIG_FAIL-- Failure in Transfer Configuration.
*  - QAPI_SPIM_ERROR_TRANSFER_FAILURE    -- Failure in Transfer.
*
*  @dependencies
*  - Before calling this API qapi_SPIM_Open() and qapi_SPIM_Power_On() API must be called.
*/
qapi_Status_t qapi_SPIM_Full_Duplex (void *spi_Handle, qapi_SPIM_Config_t *config, 
                                     qapi_SPIM_Descriptor_t *desc, uint32_t num_Descriptors,
									 qapi_SPIM_Callback_Fn_t c_Fn, void *c_Ctxt, qbool_t get_timestamp);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_CNKTD_SPI_H */


