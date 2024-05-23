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

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (XMC7200D-E272K8384 depended)
 */

#ifndef _MTKBSP_TK_SYSLIB_DEPEND_H_
#define _MTKBSP_TK_SYSLIB_DEPEND_H_

#include <tk/sysdepend/xmc_mtb/cpu/core/armv7m/syslib.h>

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Control
 */

/*
 * Interrupt number
 */
#define MIN_NVIC_INTNO	0		/* Minimum value of NVIC interrupt number */
#define MAX_NVIC_INTNO	(N_INTVEC-1)	/* Maximum value of NVIC interrupt number */

/*
 * Interrupt mode ( Use SetIntMode )
 */
#define IM_EDGE		0x0000		/* Edge trigger */
#define IM_HI		0x0002		/* Interrupt at rising edge */
#define IM_LOW		0x0001		/* Interrupt at falling edge */
#define IM_BOTH		0x0003		/* Interrupt at both edge */

#endif /* _MTKBSP_TK_SYSLIB_DEPEND_H_ */
