/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/08.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (STM32H5 depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/stm32_cube/cpu/core/armv8m/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* STM32H533 Internal SRAM 1-3   0x20000000 - 0x20043FFF  (Size 272KB) */
#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_SIZE       0x00044000

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
#define N_INTVEC		133	/* Number of Interrupt vectors */

/*
 * Exception vector table alignment
*/
#define	EXCTBL_ALIGN		1024

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
 * EXTI (Extended interrupt controller)
 */
#define	N_EXTIEVT		59	/*  Number of EXTI event input */

#if (TRUSTZONE_ENABLE && !TRUSTZONE_SECURE)
#define	MTK_EXTI_BASE	0x44022000	// Non-Secure
#else
#define MTK_EXTI_BASE	0x54022000	// Secure or TrustZone-disabled
#endif	/* TRUSTZONE_ENABLE && TRUSTZONE_SECURE */

#define	EXTI_RTSR1	(MTK_EXTI_BASE + 0x00)
#define	EXTI_FTSR1	(MTK_EXTI_BASE + 0x04)
#define	EXTI_SWIER1	(MTK_EXTI_BASE + 0x08)
#define	EXTI_RPR1	(MTK_EXTI_BASE + 0x0C)
#define	EXTI_FPR1	(MTK_EXTI_BASE + 0x10)
#define	EXTI_SECCFGR1	(MTK_EXTI_BASE + 0x14)
#define EXTI_PRIVCFGR1	(MTK_EXTI_BASE + 0x18)

#define	EXTI_RTSR2	(MTK_EXTI_BASE + 0x20)
#define	EXTI_FTSR2	(MTK_EXTI_BASE + 0x24)
#define	EXTI_SWIER2	(MTK_EXTI_BASE + 0x28)
#define	EXTI_RPR2	(MTK_EXTI_BASE + 0x2C)
#define	EXTI_FPR2	(MTK_EXTI_BASE + 0x30)
#define	EXTI_SECCFGR2	(MTK_EXTI_BASE + 0x34)
#define EXTI_PRIVCFGR2	(MTK_EXTI_BASE + 0x38)

#define	EXTI_EXTICR1	(MTK_EXTI_BASE + 0x60)
#define	EXTI_EXTICR2	(MTK_EXTI_BASE + 0x64)
#define	EXTI_EXTICR3	(MTK_EXTI_BASE + 0x68)
#define	EXTI_EXTICR4	(MTK_EXTI_BASE + 0x6C)

#define	EXTI_LOCKR	(MTK_EXTI_BASE + 0x70)

#define	EXTI_IMR1	(MTK_EXTI_BASE + 0x80)
#define	EXTI_EMR1	(MTK_EXTI_BASE + 0x84)

#define	EXTI_IMR2	(MTK_EXTI_BASE + 0x90)
#define	EXTI_EMR2	(MTK_EXTI_BASE + 0x94)

#define	EXTI_RTSR(n)	(MTK_EXTI_BASE + 0x00 +(0x20)*(n))
#define	EXTI_FTSR(n)	(MTK_EXTI_BASE + 0x04 +(0x20)*(n))
#define EXTI_RPR(n)	(MTK_EXTI_BASE + 0x0C +(0x20*(n)))	
#define EXTI_FPR(n)	(MTK_EXTI_BASE + 0x10 +(0x20*(n)))	
#define EXTI_IMR(n)	(MTK_EXTI_BASE + 0x80 +(0x10*(n)))

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

/*
 * RCC (Reset & Clock control) registers (Used from physical timer)
 */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#if (TRUSTZONE_ENABLE && !TRUSTZONE_SECURE)
	/* Non-Secure Address */
#define	MTK_GPIOA_BASE	0x42020000
#define	MTK_GPIOB_BASE	0x42020400
#define	MTK_GPIOC_BASE	0x42020800
#define	MTK_GPIOD_BASE	0x42020C00
#define	MTK_GPIOE_BASE	0x42021000
#define	MTK_GPIOF_BASE	0x42021400
#define	MTK_GPIOG_BASE	0x42021800
#define	MTK_GPIOH_BASE	0x42021C00

#else	/* Secure or TrustZone-disabled */

#define	MTK_GPIOA_BASE	0x52020000
#define	MTK_GPIOB_BASE	0x52020400
#define	MTK_GPIOC_BASE	0x52020800
#define	MTK_GPIOD_BASE	0x52020C00
#define	MTK_GPIOE_BASE	0x52021000
#define	MTK_GPIOF_BASE	0x52021400
#define	MTK_GPIOG_BASE	0x52021800
#define	MTK_GPIOH_BASE	0x52021C00

#endif	/* TRUSTZONE_ENABLE && TRUSTZONE_SECURE */

#define GPIO_MODER(n)	(MTK_GPIO##n##_BASE + 0x00)		/* GPIO port mode register */
#define GPIO_OTYPER(n)	(MTK_GPIO##n##_BASE + 0x04)		/* GPIO port output type register */
#define GPIO_OSPEEDR(n)	(MTK_GPIO##n##_BASE + 0x08)		/* GPIO port output speed register */
#define GPIO_PUPDR(n)	(MTK_GPIO##n##_BASE + 0x0C)		/* GPIO port pull-up/pull-down register */
#define GPIO_IDR(n)	(MTK_GPIO##n##_BASE + 0x10)		/* GPIO port input data register */
#define GPIO_ODR(n)	(MTK_GPIO##n##_BASE + 0x14)		/* GPIO port output data register */
#define GPIO_BSRR(n)	(MTK_GPIO##n##_BASE + 0x18)		/* GPIO port bit set/reset register */
#define GPIO_LCKR(n)	(MTK_GPIO##n##_BASE + 0x1C)		/* GPIO port configuration lock register */
#define GPIO_AFRL(n)	(MTK_GPIO##n##_BASE + 0x20)		/* GPIO alternate function low register */
#define GPIO_AFRH(n)	(MTK_GPIO##n##_BASE + 0x24)		/* GPIO alternate function high register */
#define GPIO_BRR(n)	(MTK_GPIO##n##_BASE + 0x28)		/* GPIO port bit reset register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for STM32H5)
 */
#define	CPU_HAS_PTMR	0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
