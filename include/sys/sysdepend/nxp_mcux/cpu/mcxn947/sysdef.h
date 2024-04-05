/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2024 by Ken Sakamura.
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
 *	System dependencies definition (MCX N947 depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/nxp_mcux/cpu/core/armv8m/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* MCX N947 Internal SRAM 20000000 - 0x2005FFFF  (Size 384KB) */
#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_SIZE       0x00060000

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

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

#define N_INTVEC		156	/* Number of Interrupt vectors */

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
 * GPIO
 */
#define	MTK_GPIO0_BASE	0x40096000
#define	MTK_GPIO1_BASE	0x40098000
#define	MTK_GPIO2_BASE	0x4009A000
#define	MTK_GPIO3_BASE	0x4009C000
#define	MTK_GPIO4_BASE	0x4009E000
#define	MTK_GPIO5_BASE	0x40040000

#define	GPIO_PDOR(n)	(MTK_GPIO##n##_BASE + 0x40)
#define	GPIO_PSOR(n)	(MTK_GPIO##n##_BASE + 0x44)
#define	GPIO_PCOR(n)	(MTK_GPIO##n##_BASE + 0x48)
#define	GPIO_PTOR(n)	(MTK_GPIO##n##_BASE + 0x4C)
#define	GPIO_PDIR(n)	(MTK_GPIO##n##_BASE + 0x50)

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for MCX N947)
 */
#define	CPU_HAS_PTMR	0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
