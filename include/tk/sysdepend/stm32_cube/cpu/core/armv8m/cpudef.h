/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpudef.h
 *
 *	CPU dependent definition  (ARMv8-M)
 */

#ifndef _MTKBSP_TK_CPUDEF_CORE_H_
#define _MTKBSP_TK_CPUDEF_CORE_H_

/*
 * Using FPU (depend on CPU)
 *   TA_COP0		FPU ( = TA_FPU)
 */

#if USE_FPU
#define	TA_COPS		TA_COP0
#else
#define	TA_COPS		0
#endif

#define TA_FPU		TA_COP0		/* dummy. An error occurs when checking API calls. */

/*
 * General purpose register		tk_get_reg tk_set_reg
 */
typedef struct t_regs {
	VW	r[13];		/* General purpose register R0-R12 */
	void	*lr;		/* Link register R14 */
} T_REGS;

/*
 * Exception-related register		tk_get_reg tk_set_reg
 */
typedef struct t_eit {
	void	*pc;		/* Program counter R15 */
	UW	xpsr;		/* Program status register */
	UW	taskmode;	/* Task mode flag */
} T_EIT;

/*
 * Control register			tk_get_reg tk_set_reg
 */
typedef struct t_cregs {
	void	*ssp;		/* System stack pointer R13_svc */
} T_CREGS;

#if NUM_COPROCESSOR > 0
/*
 * Co-processor register
 */
typedef struct t_copregs {
	VW	s[32];		/* FPU General purpose register S0-S31 */
	UW	fpscr;		/* Floating-point Status and Control Register */
} T_COPREGS;
#endif  /* NUM_COPROCESSOR  */

#endif /* _MTKBSP_TK_CPUDEF_CORE_H_ */
