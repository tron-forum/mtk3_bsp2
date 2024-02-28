/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/01.
 *
 *----------------------------------------------------------------------
 */

#ifndef	_DEV_HAL_DEVICE_H_
#define	_DEV_HAL_DEVICE_H_
/*
 *	device.h
 *	HAL device driver (MCUXpresso)
*/
#include "peripherals.h"
#include <config_bsp/nxp_mcux/config_bsp.h>

#if DEVCNF_USE_HAL_ADC
#include <sysdepend/nxp_mcux/device/hal_adc/hal_adc.h>
#endif

#if DEVCNF_USE_HAL_LPI2C
#include <sysdepend/nxp_mcux/device/hal_lpi2c/hal_lpi2c.h>
#endif

#endif	/* _DEV_HAL_DEVICE_H_ */