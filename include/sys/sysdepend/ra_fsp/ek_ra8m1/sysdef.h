/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-24 by Ken Sakamura.
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
 *	System dependencies definition (EK-RA8M1)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_H_

/* CPU-dependent definition */
#include <sys/sysdepend/ra_fsp/cpu/ra8m1/sysdef.h>


/* ------------------------------------------------------------------------ */
/* Clock frequency
 */
#define CPUCLK_MHz	(480)
#define ICLK_MHz	(240)
#define PCLKA_MHz	(120)
#define PCLKB_MHz	(60)
#define PCLKC_MHz	(60)
#define PCLKD_MHz	(120)
#define PCLKE_MHz	(240)
#define	SDCLK_MHz	(120)
#define	BCLK_MHz	(120)
#define	EBCLK_MHz	(60)
#define	FCLK_MHz	(60)

#define	SYSCLK		(CPUCLK_MHz*1000*1000)	// System clock (Hz)
#define TMCLK_KHz	(CPUCLK_MHz*1000)	// System timer clock input (kHz)
#define TMCLK		(CPUCLK_MHz)		// System timer clock input (MHz)

#endif /* _MTKBSP_TK_SYSDEF_DEPEND_H_ */
