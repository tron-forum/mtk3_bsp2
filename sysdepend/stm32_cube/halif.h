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

/*
 *	halif.h (STM32Cube)
 *	HAL I/F
 */
#ifndef _MTKBSP_SYSDEPEND_HALIF_
#define _MTKBSP_SYSDEPEND_HALIF_
#include <stdint.h>


/* Get clock frequency (CMSIS wrapper function)*/

uint32_t HAL_RCC_GetSysClockFreq(void);
uint32_t HAL_RCC_GetHCLKFreq(void);
uint32_t HAL_RCC_GetPCLK1Freq(void);
uint32_t HAL_RCC_GetPCLK2Freq(void);

Inline uint32_t halif_get_sysclk(void)
{
	return HAL_RCC_GetSysClockFreq();
}

Inline uint32_t halif_get_hclk(void)
{
	return HAL_RCC_GetHCLKFreq();
}

Inline uint32_t halif_get_pclk1(void)
{
	return HAL_RCC_GetPCLK1Freq();
}

Inline uint32_t halif_get_pclk2(void)
{
	return HAL_RCC_GetPCLK2Freq();
}

#endif	/* _MTKBSP_SYSDEPEND_HALIF_ */