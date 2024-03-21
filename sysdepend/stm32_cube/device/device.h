/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */

#ifndef	_DEV_HAL_DEVICE_H_
#define	_DEV_HAL_DEVICE_H_
/*
 *	device.h
 *	HAL device driver (STM32Cube FW)
*/
#if MTKBSP_CPU_STM32F4
#include <stm32f4xx_hal.h>

#elif MTKBSP_CPU_STM32L4
#include <stm32l4xx_hal.h>

#elif MTKBSP_CPU_STM32G4
#include <stm32g4xx_hal.h>

#elif MTKBSP_CPU_STM32H7
#include <stm32h7xx_hal.h>

#elif MTKBSP_CPU_STM32F7
#include <stm32f7xx_hal.h>
#endif

#include <config_bsp/stm32_cube/config_bsp.h>

#if DEVCNF_USE_HAL_IIC
#include <sysdepend/stm32_cube/device/hal_i2c/hal_i2c.h>
#endif

#if DEVCNF_USE_HAL_ADC
#include <sysdepend/stm32_cube/device/hal_adc/hal_adc.h>
#endif

#endif	/* _DEV_HAL_DEVICE_H_ */