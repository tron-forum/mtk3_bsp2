/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

#ifndef	_DEV_HAL_DEVICE_H_
#define	_DEV_HAL_DEVICE_H_
/*
 *	device.h
 *	HAL device driver (XMC7200 MTB)
*/

#include <config_bsp/xmc_mtb/config_bsp.h>

#if DEVCNF_USE_HAL_IIC
#include <sysdepend/xmc_mtb/device/hal_i2c/hal_i2c.h>
#endif

#if DEVCNF_USE_HAL_ADC
#include <sysdepend/xmc_mtb/device/hal_adc/hal_adc.h>
#endif

#endif	/* _DEV_HAL_DEVICE_H_ */