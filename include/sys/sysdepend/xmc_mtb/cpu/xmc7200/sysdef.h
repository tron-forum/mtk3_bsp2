/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (XMC7200 depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYSDEF_DEPEND_CPU_H__
#define _MTKBSP_SYSDEF_DEPEND_CPU_H__

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/xmc_mtb/cpu/core/armv7m/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Disable the definition in uT-Kernel 3.0 and use the functions of ModusToolbox.
 */
#undef SCB_SCR


/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* XMC7200 Internal SRAM	0x28000000 - 0x280FFFFF	1,024KB	*/
/*		for Cortex-M0+	0x28000000 - 0x28004000		16KB	*/
/*		for Cortex-M7_0	0x28004000 - 0x280F0000		944KB	*/
/*		for Cortex-M7_1	0x280F0000 - 0x28100000		64KB	*/
#define INTERNAL_RAM_SIZE       0x000EC000
#define INTERNAL_RAM_START      0x28004000
#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

#define	SYSTEM_INT_IDX_MASK	(0x000003FFU)
#define	SYSTEM_INT_VALID	(1U << 31)
#define SYSTEM_INT_VALID_Pos	(31U)

#define	CPU_INT_IDX(x)	((x) << 0)
#define	CPU_INT_VALID	(1U << 31)

/* ------------------------------------------------------------------------ */
/*
 * Peripheral PCLK groups (PERI_PCLK)
 */
#define	PERI_PCLK_GR0_BASE		(0x40040000)
#define	PERI_PCLK_GR0_DIV_CMD	(_UW*)(PERI_PCLK_GR0_BASE+0x0000ul)		/* Divider command Register */


#define	PERI_PCLK_GR1_BASE			(0x40042000)
#define	PERI_PCLK_GR1_DIV_CMD		(_UW*)(PERI_PCLK_GR1_BASE+0x0000ul)				/* Divider command Register */
#define	PERI_PCLK_GR1_CLOCK_CTL(x)	(_UW*)(PERI_PCLK_GR1_BASE+0x0C00ul+((x)*0x4ul))	/* Divider command Register */
#define	PERI_PCLK_GR1_DIV_16_CTL(x)	(_UW*)(PERI_PCLK_GR1_BASE+0x1400ul+((x)*0x4ul))	/* Divider control Register */

#define	DIV_SEL(x)			((x) << 0)	/* Specifies one of the dividers of the divider type specified by TYPE_SEL. */
#define	DIV_SEL_MASK		(0x000000FF)
#define	TYPE_SEL_8			(0x0 << 8)	/* Specifies divider type:8.0 (integer) clock dividers. */
#define	TYPE_SEL_16			(0x1 << 8)	/* Specifies divider type:16.0 (integer) clock dividers. */
#define	TYPE_SEL_16_5		(0x2 << 8)	/* Specifies divider type:16.5 (fractional) clock dividers. */
#define	TYPE_SEL_24_5		(0x3 << 8)	/* Specifies divider type:24.5 (fractional) clock dividers. */
#define	TYPE_SEL_MASK		(0x00000300)
#define	DIV_CMD_DISABLE		(1U << 30)
#define	DIV_CMD_ENABLE		(1U << 31)

#define	DIV_16_CTRL_EN		(1U << 0)	/* Divider enabled. */
#define	DIV_16_CTRL_INT16_DIV(x)	((x) << 8)	/* Integer division by (1+INT16_DIV). */

/* ------------------------------------------------------------------------ */
/*
 * System Timer clock
 */

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50


/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		575	/* Number of Interrupt vectors */
							/* External Interrupt 567(0-566). Software Interrupt 8. */
							/* 567- is assigned to Software Interrupt */
#define	N_SYSVEC		16	/* Number of System Exceptions */

#define	CORE_EXT_INTVEC	8	/* Externel Interrupt number is 0-7. */
#define	CORE_SOFT_INTVEC	8	/* Software Interrupt number is 8-15. */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		3


/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
#define INTPRI_PENDSV		7	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for XMC7200)
 */
#define	CPU_HAS_PTMR		(118)

/* Phycail timer Maximum count */
#define TCPWM_MAX_CNT16		(0x0000FFFF)
#define TCPWM_MAX_CNT32		(0xFFFFFFFF)

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		1
#define CPU_HAS_DPS		0

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#if USE_FPU
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif

#endif /* _MTKBSP_SYSDEF_DEPEND_CPU_H__ */
