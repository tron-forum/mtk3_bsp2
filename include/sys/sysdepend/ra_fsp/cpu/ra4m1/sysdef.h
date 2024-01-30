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
 *	System dependencies definition (RA4M1 depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/ra_fsp/cpu/core/armv7m/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* RA4M1 Internal SRAM   0x20000000 - 0x20007FFF  (Size 32KB)     */
#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_SIZE       0x00008000

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

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
#define	N_SYSVEC		16	/* Number of System Exceptions */

#define N_INTVEC		32	/* Number of Interrupt vectors */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		4

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
#define INTPRI_PENDSV		15	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		1
#define CPU_HAS_DSP		0

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#if USE_FPU
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif

/* ------------------------------------------------------------------------ */
/*
 * CPU stack pointer Monitor
 */
/* Main Stack */
#define SPMON_MSPMPUOAD		(0x40000D00)	// Action after detection reg.
#define SPMON_MSPMPUCTL		(0x40000D04)	// Control reg.
#define SPMON_MSPMPUPT		(0x40000D06)	// Protect reg.
#define SPMON_MSPMPUSA		(0x40000D08)	// Start address reg.
#define SPMON_MSPMPUEA		(0x40000D0C)	// End address reg.
/* Process Stack */
#define SPMON_PSPMPUOAD		(0x40000D10)	// Action after detection reg.
#define SPMON_PSPMPUCTL		(0x40000D14)	// Control reg.
#define SPMON_PSPMPUPT		(0x40000D16)	// Protect reg. 
#define SPMON_PSPMPUSA		(0x40000D18)	// Start address reg.
#define SPMON_PSPMPUEA		(0x40000D1C)	// End address reg.

/* ------------------------------------------------------------------------ */
/*
 * PORT
 */
#define MTK_PORT0_BASE		(0x40040000)
#define MTK_PORT1_BASE		(0x40040020)
#define MTK_PORT2_BASE		(0x40040040)
#define MTK_PORT3_BASE		(0x40040060)
#define MTK_PORT4_BASE		(0x40040080)
#define MTK_PORT5_BASE		(0x400400A0)
#define MTK_PORT6_BASE		(0x400400C0)
#define MTK_PORT7_BASE		(0x400400E0)
#define MTK_PORT8_BASE		(0x40040100)
#define MTK_PORT9_BASE		(0x40040120)

#define PORT_PODR(n)		(MTK_PORT##n##_BASE + 0x00)	/* Port output data register */
#define PORT_PIDR(n)		(MTK_PORT##n##_BASE + 0x06)	/* Port input data register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer
 */
#define	CPU_HAS_PTMR	0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
