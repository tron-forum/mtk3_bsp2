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
 *	sysdef.h
 *
 *	System dependencies definition (Arduino UNO R4)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_H_

/* CPU-dependent definition */
#include <sys/sysdepend/ra_fsp/cpu/ra4m1/sysdef.h>


/* ------------------------------------------------------------------------ */
/* Clock frequency
 */
#define ICLK_MHz	(48)
#define PCLKA_MHz	(48)
#define PCLKB_MHz	(24)
#define PCLKC_MHz	(48)
#define PCLKD_MHz	(48)
#define	FCLK_MHz	(24)

#define	SYSCLK		(ICLK_MHz*1000000)	// System clock (Hz)
#define TMCLK_KHz	(ICLK_MHz*1000)		// System timer clock input (kHz)
#define TMCLK		(ICLK_MHz)		// System timer clock input (MHz)

#endif /* _MTKBSP_TK_SYSDEF_DEPEND_H_ */
