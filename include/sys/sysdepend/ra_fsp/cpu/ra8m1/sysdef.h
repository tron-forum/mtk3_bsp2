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
 *	System dependencies definition (RA8M1 depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/ra_fsp/cpu/core/armv8m/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* RA8M1 Internal SRAM   0x22000000 - 0x220DFFFF  (Size 896KB)     */
#define INTERNAL_RAM_START      0x22000000
#define INTERNAL_RAM_SIZE       0x000E0000

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

#define N_INTVEC		96	/* Number of Interrupt vectors */

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
 * PORT
 */
#define MTK_PORT0_BASE		(0x40400000)
#define MTK_PORT1_BASE		(0x40400020)
#define MTK_PORT2_BASE		(0x40400040)
#define MTK_PORT3_BASE		(0x40400060)
#define MTK_PORT4_BASE		(0x40400080)
#define MTK_PORT5_BASE		(0x404000A0)
#define MTK_PORT6_BASE		(0x404000C0)
#define MTK_PORT7_BASE		(0x404000E0)
#define MTK_PORT8_BASE		(0x40400100)
#define MTK_PORT9_BASE		(0x40400120)
#define MTK_PORTA_BASE		(0x40400140)
#define MTK_PORTB_BASE		(0x40400160)

#define PORT_PODR(n)		(MTK_PORT##n##_BASE + 0x02)	/* Port output data register */
#define PORT_PIDR(n)		(MTK_PORT##n##_BASE + 0x06)	/* Port input data register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer
 */
#define	CPU_HAS_PTMR	0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
