/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef MTKBSP_STM32CUBE

/*
 *	devinit.c (STM32Cube)
 *	Device-Dependent Initialization
 */
#include <tk/tkernel.h>
#include <tk/device.h>
#include <kernel.h>

/* ------------------------------------------------------------------------ */

/*
 * Initialization before micro T-Kernel starts
 */

EXPORT ER knl_init_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Start processing after T-Kernel starts
 *	Called from the initial task contexts.
 */
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

#if DEVCNF_USE_HAL_IIC
	IMPORT I2C_HandleTypeDef	hi2c1;

	err = dev_init_hal_i2c(DEV_HAL_I2C1, &hi2c1);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_ADC
	IMPORT ADC_HandleTypeDef	hadc1;
	err = dev_init_hal_adc(DEV_HAL_ADC1, &hadc1);
	if(err < E_OK) return err;

#if defined(MTKBSP_NUCLEO_STM32H723) || defined(MTKBSP_NUCLEO_STM32F767)
	IMPORT ADC_HandleTypeDef	hadc3;
	
	err = dev_init_hal_adc(DEV_HAL_ADC2, NULL);	// hadc2 is not used
	if(err < E_OK) return err;
	err = dev_init_hal_adc(DEV_HAL_ADC3, &hadc3);
	if(err < E_OK) return err;

#elif defined(MTKBSP_NUCLEO_STM32G431) || defined(MTKBSP_NUCLEO_STM32G491)
	IMPORT ADC_HandleTypeDef	hadc2;
	
	err = dev_init_hal_adc(DEV_HAL_ADC2, &hadc2);
	if(err < E_OK) return err;
	err = dev_init_hal_adc(DEV_HAL_ADC3, NULL);	// hadc3 is not used.
	if(err < E_OK) return err;
#else
//#elif defined(MTKBSP_NUCLEO_STM32L476) || defined(MTKBSP_NUCLEO_STM32F401) || defined(MTKBSP_NUCLEO_STM32F411) || defined(MTKBSP_NUCLEO_STM32F446)
	err = dev_init_hal_adc(DEV_HAL_ADC2, NULL);	// hadc2 is not used
	if(err < E_OK) return err;
	err = dev_init_hal_adc(DEV_HAL_ADC3, NULL);	// hadc3 is not used
	if(err < E_OK) return err;

#endif
#endif	/* DEVCNF_USE_HAL_ADC */

	return err;
}

#if USE_SHUTDOWN
/* ------------------------------------------------------------------------ */
/*
 * System finalization
 *	Called just before system shutdown.
 *	Execute finalization that must be done before system shutdown.
 */
EXPORT ER knl_finish_device( void )
{
	return E_OK;
}

#endif /* USE_SHUTDOWN */
#endif /* MTKBSP_STM32CUBE */
