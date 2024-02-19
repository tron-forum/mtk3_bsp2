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

#ifndef	_DEV_HAL_DEVICE_H_
#define	_DEV_HAL_DEVICE_H_
/*
 *	device.h
 *	HAL device driver (FSP)
*/

#include <hal_data.h>
#include <config_bsp/ra_fsp/config_bsp.h>

#if DEVCNF_USE_HAL_ADC
#include <sysdepend/ra_fsp/device/hal_adc/hal_adc.h>
#endif

#if DEVCNF_USE_HAL_IIC
#include <sysdepend/ra_fsp/device/hal_i2c/hal_i2c.h>
#endif

#if DEVCNF_USE_HAL_SCI_IIC
#include <sysdepend/ra_fsp/device/hal_sci_i2c/hal_sci_i2c.h>
#endif

#if DEVCNF_USE_HAL_I3C_IIC
#include <sysdepend/ra_fsp/device/hal_i3c_i2c/hal_i3c_i2c.h>
#endif

#endif	/* _DEV_HAL_DEVICE_H_ */