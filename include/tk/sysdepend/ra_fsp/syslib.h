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
 *	syslib.h
 *
 *	micro T-Kernel System Library  (RA FSP)
 */

#ifndef _MTKBSP_TK_SYSLIB_DEPEND_H_
#define _MTKBSP_TK_SYSLIB_DEPEND_H_

#if MTKBSP_CPU_CORE_ARMV7M
#include <tk/sysdepend/ra_fsp/cpu/core/armv7m/syslib.h>

#elif MTKBSP_CPU_CORE_ARMV8M
#include <tk/sysdepend/ra_fsp/cpu/core/armv8m/syslib.h>
#endif

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Control
 */

/*
 * Interrupt number
 */
#define MIN_NVIC_INTNO	0		/* Minimum value of NVIC interrupt number */
#define MAX_NVIC_INTNO	(N_INTVEC-1)	/* Maximum value of NVIC interrupt number */

#endif /* _MTKBSP_TK_SYSLIB_DEPEND_H_ */
