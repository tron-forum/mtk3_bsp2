/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (STM32Cule NUCLEO STM32L476)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_H_

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */
#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_SIZE       0x00018000	/* SRAM1 Size 96KB */
#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Interruptrs
 */
#define N_INTVEC		82	/* Number of Interrupt vectors */
#define	N_EXTIEVT		40	/*  Number of EXTI event input */

/* ------------------------------------------------------------------------ */
/* STM32L4 CPU-dependent definition
 */
#include <sys/sysdepend/stm32_cube/cpu/stm32l4/sysdef.h>

#endif /* _MTKBSP_TK_SYSDEF_DEPEND_H_ */
