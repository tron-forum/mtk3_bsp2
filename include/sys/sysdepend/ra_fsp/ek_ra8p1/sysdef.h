/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (EK-RA8P1)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_H_

/* CPU-dependent definition */
#include <sys/sysdepend/ra_fsp/cpu/ra8m1/sysdef.h>


/* ------------------------------------------------------------------------ */
/* Clock frequency
 */
#define CPUCLK_MHz	(1000)
#define ICLK_MHz	(250)
#define PCLKA_MHz	(125)
#define PCLKB_MHz	(62)
#define PCLKC_MHz	(125)
#define PCLKD_MHz	(250)
#define PCLKE_MHz	(250)

#define	SYSCLK		(CPUCLK_MHz*1000*1000)	// System clock (Hz)
#define TMCLK_KHz	(CPUCLK_MHz*1000)	// System timer clock input (kHz)
#define TMCLK		(CPUCLK_MHz)		// System timer clock input (MHz)

#endif /* _MTKBSP_TK_SYSDEF_DEPEND_H_ */
