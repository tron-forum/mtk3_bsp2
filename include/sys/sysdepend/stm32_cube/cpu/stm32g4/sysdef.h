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
 *	System dependencies definition (STM32G4 depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/stm32_cube/cpu/core/armv7m/sysdef.h>

/* ------------------------------------------------------------------------
 * CPU-dependent definition (STM32G4)
 */
/*
 * Internal Memorie (Main RAM)
 */

/* STM32G431RB Internal SRAM1&2   0x20000000 - 0x200057FF  (Size 22KB) */
/* STM32G491RE Internal SRAM1&2   0x20000000 - 0x20017FFF  (Size 96KB) */

#define INTERNAL_RAM_START      0x20000000

#if defined(MTKBSP_CPU_STM32G431)
#define INTERNAL_RAM_SIZE       0x00005800	/* STM32G431 SRAM1 size 22KB */
#elif defined(MTKBSP_CPU_STM32G491)
#define INTERNAL_RAM_SIZE       0x00018000	/* STM32G491 SRAM1 size 96KB */
#endif

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
#define N_INTVEC		102	/* Number of Interrupt vectors */

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
#define	N_EXTIEVT		42	/*  Number of EXTI event input */

#define MTK_EXTI_BASE	0x40010400

#define	EXTI_IMR1	(MTK_EXTI_BASE + 0x00)
#define	EXTI_EMR1	(MTK_EXTI_BASE + 0x04)
#define	EXTI_RTSR1	(MTK_EXTI_BASE + 0x08)
#define	EXTI_FTSR1	(MTK_EXTI_BASE + 0x0C)
#define	EXTI_SWIER1	(MTK_EXTI_BASE + 0x10)
#define	EXTI_PR1	(MTK_EXTI_BASE + 0x14)
#define	EXTI_IMR2	(MTK_EXTI_BASE + 0x20)
#define	EXTI_EMR2	(MTK_EXTI_BASE + 0x24)
#define	EXTI_RTSR2	(MTK_EXTI_BASE + 0x28)
#define	EXTI_FTSR2	(MTK_EXTI_BASE + 0x2C)
#define	EXTI_SWIER2	(MTK_EXTI_BASE + 0x30)
#define	EXTI_PR2	(MTK_EXTI_BASE + 0x34)

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
 * RCC (Reset & Clock control) registers
 */
#define MTK_RCC_BASE	0x40021000
#define RCC_AHB1ENR	(MTK_RCC_BASE + 0x0048)	/* AHB1 peripheral clock enable register */
#define RCC_AHB2ENR	(MTK_RCC_BASE + 0x004C)	/* AHB2 peripheral clock enable register */
#define RCC_AHB3ENR	(MTK_RCC_BASE + 0x0050)	/* AHB3 peripheral clock enable register */
#define RCC_APB1ENR1	(MTK_RCC_BASE + 0x0058)	/* APB1 peripheral clock enable register 1 */
#define RCC_APB1ENR2	(MTK_RCC_BASE + 0x005C)	/* APB1 peripheral clock enable register 2 */
#define RCC_APB2ENR	(MTK_RCC_BASE + 0x0060)	/* APB2 peripheral clock enable register */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	MTK_GPIOA_BASE	0x48000000
#define	MTK_GPIOB_BASE	0x48000400
#define	MTK_GPIOC_BASE	0x48000800
#define	MTK_GPIOD_BASE	0x48000C00
#define	MTK_GPIOE_BASE	0x48001000
#define	MTK_GPIOF_BASE	0x48001400
#define	MTK_GPIOG_BASE	0x48001800

#define GPIO_MODER(n)	(MTK_GPIO##n##_BASE + 0x00)	/* GPIO port mode register */
#define GPIO_OTYPER(n)	(MTK_GPIO##n##_BASE + 0x04)	/* GPIO port output type register */
#define GPIO_OSPEEDR(n)	(MTK_GPIO##n##_BASE + 0x08)	/* GPIO port output speed register */
#define GPIO_PUPDR(n)	(MTK_GPIO##n##_BASE + 0x0C)	/* GPIO port pull-up/pull-down register */
#define GPIO_IDR(n)	(MTK_GPIO##n##_BASE + 0x10)	/* GPIO port input data register */
#define GPIO_ODR(n)	(MTK_GPIO##n##_BASE + 0x14)	/* GPIO port output data register */
#define GPIO_BSRR(n)	(MTK_GPIO##n##_BASE + 0x18)	/* GPIO port bit set/reset register */
#define GPIO_LCKR(n)	(MTK_GPIO##n##_BASE + 0x1C)	/* GPIO port configuration lock register */
#define GPIO_AFRL(n)	(MTK_GPIO##n##_BASE + 0x20)	/* GPIO alternate function low register */
#define GPIO_AFRH(n)	(MTK_GPIO##n##_BASE + 0x24)	/* GPIO alternate function high register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (use TIM2-TIM5)
 */
#define	CPU_HAS_PTMR	(1)

/* Timer register definition */
#define	MTK_TIM2_BASE	0x40000000
#define	MTK_TIM3_BASE	0x40000400

#define	TIMxCR1		0x00
#define TIMxCR2		0x04
#define TIMxSMCR	0x08
#define TIMxDIER	0x0C
#define TIMxSR		0x10
#define TIMxEGR		0x14
#define TIMxCCMR1	0x18
#define TIMxCCMR2	0x1C
#define TIMxCCER	0x20
#define TIMxCNT		0x24
#define TIMxPSC		0x28
#define TIMxARR		0x2C
#define TIMxCCR1	0x34
#define TIMxCCR2	0x38
#define TIMxCCR3	0x3C
#define TIMxCCR4	0x40
#define TIMxDCR		0x48
#define TIMxDMAR	0x4C

#define	TIMxCR1_CEN	(1<<0)
#define	TIMxCR1_OPM	(1<<3)
#define	TIMxCR1_DIR	(1<<4)
#define	TIMxDIER_UIE	(1<<0)
#define TIMxSR_UIF	(1<<0)
#define TIMxEGR_UG	(1<<0)

/* Prescaler value */
#define TIM2PSC_PSC_INIT	0
#define TIM3PSC_PSC_INIT	0

/* Physical timer interrupt number */
#define INTNO_TIM2	28
#define INTNO_TIM3	29

/* Physical timer interrupt priority */
#define INTPRI_TIM2	5
#define INTPRI_TIM3	5

/* Phycail timer Maximum count */
#define PTMR_MAX_CNT16    (0x0000FFFF)
#define PTMR_MAX_CNT32    (0xFFFFFFFF)

#endif /* _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_ */
