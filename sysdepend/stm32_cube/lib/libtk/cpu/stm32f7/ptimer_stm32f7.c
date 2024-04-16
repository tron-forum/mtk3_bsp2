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
#ifdef MTKBSP_CPU_STM32F7

/*
 *	ptimer_stm32f7.c
 *
 *	Physical timer (STM32CUBE STM32F7)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>
#include <sysdepend/stm32_cube/halif.h>

#if USE_PTMR

/*
 * Initialize physical timer
 */
EXPORT void knl_init_ptmr(void)
{
	out_w(RCC_APB1ENR, in_w(RCC_APB1ENR)|0x00000003);	// Enable TIM2-TIM3
	while((in_w(RCC_APB1ENR)&0x00000003)!=0x00000003);
}

#endif	/* USE_PTMR */
#endif	/* MTKBSP_CPU_STM32F7 */