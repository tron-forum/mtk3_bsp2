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
 *	profile.h
 *
 *	Service Profile (NUCLEO STM32F411)
 */

#ifndef _MTKBSP_SYS_DEPEND_PROFILE_H_
#define _MTKBSP_SYS_DEPEND_PROFILE_H_

/*
 **** CPU Core depended profile (ARMv7M)
 */
#include <sys/sysdepend/stm32_cube/cpu/core/armv7m/profile.h>

/*
 **** Target-depeneded profile (STM32Cude NUCLEO STM32F411)
 */

/*
 * Power management
 */
#define TK_SUPPORT_LOWPOWER	FALSE		/* Support of power management */

/*
 * Device Support
 */
#define TK_SUPPORT_IOPORT	TRUE		/* Support of I/O port access */

/*
 * Physical timer
 */
#if USE_PTMR
#define TK_SUPPORT_PTIMER	TRUE		/* Support of physical timer */
#define TK_MAX_PTIMER		2		/* Maximum number of physical timers. */
#else
#define TK_SUPPORT_PTIMER	FALSE		/* Support of physical timer */
#define TK_MAX_PTIMER		0		/* Maximum number of physical timers. */
#endif

#endif /* _MTKBSP_SYS_DEPEND_PROFILE_H_ */
