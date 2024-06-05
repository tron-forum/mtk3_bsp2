/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */


/*
 *	i2c_hal_xmc7200.c
 *	I2C device driver HAL layer
 *	System-dependent definition for XMC7200
 */
#include <sys/machine.h>
#include <config_bsp/xmc_mtb/config_bsp.h>

#include <cyhal.h>
#include <cybsp.h>
#include <cy_sysclk.h>
#include "i2c_sysdep.h"
#include "i2c_cnf_sysdep.h"

#ifdef MTKBSP_MODUSTOOLBOX
#if DEVCNF_USE_HAL_IIC

/* Assign divider type and number for I2C */
#define I2C_CLK_DIV_TYPE  (CY_SYSCLK_DIV_16_BIT)
#define I2C_CLK_DIV_NUMBER  (1U)

/* Allocate context for I2C operation */
cy_stc_scb_i2c_context_t i2cContext[DEV_I2C_UNITNM];
CySCB_Type *SCB_BASE_ADDRESSES[DEV_I2C_UNITNM] = {
	SCB0,
	SCB1,
	SCB2,
	SCB3,
	SCB4,
	SCB5,
	SCB6,
	SCB7,
	SCB8,
	SCB9,
	SCB10,
};

/*-------------------------------------------------------
 * Interrupt handler
 */
void i2c_hal_do_int( unsigned int unit )
{
	Cy_SCB_I2C_MasterInterrupt(SCB_BASE_ADDRESSES[unit], &i2cContext[unit]);
}

int i2c_hal_check_busy( unsigned int unit)
{
	uint32_t status = Cy_SCB_I2C_MasterGetStatus(SCB_BASE_ADDRESSES[unit], &i2cContext[unit]);

	if ((CY_SCB_I2C_MASTER_BUSY & status) == 0){
		return 0;
	}
	return 1;
}



/*----------------------------------------------------------------------
 * Set communication parameters
 */
void i2c_hal_master_write(unsigned int unit, unsigned int sadr, unsigned int sdat_num, unsigned char *sbuf, unsigned int stop)
{
	cy_stc_scb_i2c_master_xfer_config_t transfer;
	
	/* Configure write transaction */
	transfer.slaveAddress = sadr;
	transfer.buffer       = sbuf;
	transfer.bufferSize   = sdat_num;
	transfer.xferPending  = stop ? false : true; /* false = Generate Stop condition at the end of transaction */
	/* Initiate write transaction.
	* The Start condition is generated to begin this transaction.
	*/
	(void) Cy_SCB_I2C_MasterWrite(SCB_BASE_ADDRESSES[unit], &transfer, &i2cContext[unit]);
}

void i2c_hal_master_read(unsigned int unit, unsigned int sadr, unsigned int rdat_num, unsigned char *rbuf)
{
	cy_stc_scb_i2c_master_xfer_config_t transfer;
	
	/* Configure read transaction */
	transfer.slaveAddress = sadr;
	transfer.buffer       = rbuf;
	transfer.bufferSize   = rdat_num;
	transfer.xferPending  = false; /* Generate Stop condition the end of transaction */
	/* Initiate read transaction.
	* The ReStart condition is generated to begin this transaction because
	* previous transaction was completed without Stop.
	*/
	(void) Cy_SCB_I2C_MasterRead(SCB_BASE_ADDRESSES[unit], &transfer, &i2cContext[unit]);
}


/*----------------------------------------------------------------------
 * Low level clock initializ
 */
void i2c_hal_clock_init(unsigned int unit)
{
	uint16_t div;
	
	if (unit == DEV_I2C_9) {
		/* Connect assigned divider to be a clock source for I2C */
		Cy_SysClk_PeriphAssignDivider(PCLK_SCB9_CLOCK, I2C_CLK_DIV_TYPE, I2C_CLK_DIV_NUMBER);
		
		/* I2C master desired data rate is 100 kbps.
		* To support this data rate the clk_scb frequency must be in range 1.55 – 3.2 MHz.
		* Find clk_scb valid ranges in TRM section I2C sub-section Oversampling and Bit Rate.
		* For clk_peri = 100 MHz, select divider value 64 and get clk_scb = (100 MHz / 64) = 1.563 MHz.
		* This clock frequency meets requirements above.
		*/
		div = (Cy_SysClk_ClkHfGetFrequency(2) / 1563000) -1;
		Cy_SysClk_PeriphSetDivider   (I2C_CLK_DIV_TYPE, I2C_CLK_DIV_NUMBER, div);
		Cy_SysClk_PeriphEnableDivider(I2C_CLK_DIV_TYPE, I2C_CLK_DIV_NUMBER);
	}
}

/*----------------------------------------------------------------------
 * Device initialization
 */
int i2c_hal_init(unsigned int unit)
{
	unsigned int dataRate;
	
	/* Master  configuration */
	const cy_stc_scb_i2c_config_t i2cConfig =
	{
		.i2cMode   = CY_SCB_I2C_MASTER,
		.useRxFifo = false,
		.useTxFifo = true,
		.slaveAddress     = 0U,
		.slaveAddressMask = 0U,
		.acceptAddrInFifo = false,
		.ackGeneralAddr   = false,
		.enableWakeFromSleep = false,
		.enableDigitalFilter = false,
		.lowPhaseDutyCycle = 8U,
		.highPhaseDutyCycle = 8U,
	};
	
	if(unit == DEV_I2C_9) {
		/* Assign pins for I2C on SCB9: SDA = P15[1] and SCL = P15[2] */
		/* Connect SCB3 I2C function to pins */
		Cy_GPIO_Pin_Init(CYBSP_I2C_SDA_PORT, CYBSP_I2C_SDA_PIN, &CYBSP_I2C_SDA_config);
		Cy_GPIO_Pin_Init(CYBSP_I2C_SCL_PORT, CYBSP_I2C_SCL_PIN, &CYBSP_I2C_SCL_config);
		
		/* Configure I2C to operate */
		(void) Cy_SCB_I2C_Init(SCB_BASE_ADDRESSES[unit], &i2cConfig, &i2cContext[unit]);
		
		/* Configure master to operate with desired data rate */
		dataRate = Cy_SCB_I2C_SetDataRate(SCB_BASE_ADDRESSES[unit], DEVCNF_I2C_DATA_RATE_HZ, 1563000);
		if ((dataRate > DEVCNF_I2C_DATA_RATE_HZ) || (dataRate == 0U))
		{
			return -1;
		}
		
		/* Enable I2C to operate */
		Cy_SCB_I2C_Enable(SCB_BASE_ADDRESSES[unit]);
	}
	else {
		return -1;
	}


	return 0;
}

#endif		/* DEVCNF_USE_HAL_IIC */
#endif		/* MTKBSP_MODUSTOOLBOX */
