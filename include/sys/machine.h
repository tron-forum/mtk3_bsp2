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
 *	machine.h
 *	Machine type definition 
 */

#ifndef _MTKBSP_SYS_MACHINE_H_
#define _MTKBSP_SYS_MACHINE_H_

#define ADD_PREFIX_KNL_TO_GLOBAL_NAME		(1)
#define Csym(sym) sym

/* ===== System dependencies definitions ================================ */

#if defined(_STM32CUBE_NUCLEO_L476_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32l476/machine.h>

#elif defined(_STM32CUBE_NUCLEO_L4R5_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32l4r5/machine.h>

#elif defined(_STM32CUBE_NUCLEO_F401_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32f401/machine.h>

#elif defined(_STM32CUBE_NUCLEO_F411_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32f411/machine.h>

#elif defined(_STM32CUBE_NUCLEO_F446_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32f446/machine.h>

#elif defined(_STM32CUBE_NUCLEO_G431_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32g431/machine.h>

#elif defined(_STM32CUBE_NUCLEO_G491_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32g491/machine.h>

#elif defined(_STM32CUBE_NUCLEO_F767_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32f767/machine.h>

#elif defined(_STM32CUBE_NUCLEO_H723_)
#include <sys/sysdepend/stm32_cube/nucleo_stm32h723/machine.h>

#elif defined(_RAFSP_EK_RA6M3_)
#include <sys/sysdepend/ra_fsp/ek_ra6m3/machine.h>

#elif defined(_RAFSP_EK_RA8M1_)
#include <sys/sysdepend/ra_fsp/ek_ra8m1/machine.h>

#elif defined(_RAFSP_ARDUINO_UNOR4_)
#include <sys/sysdepend/ra_fsp/arduino_unor4/machine.h>

#elif defined(_NXPMCUX_EVK_LPC55S69_)
#include <sys/sysdepend/nxp_mcux/evk_lpc55s69/machine.h>

#elif defined(_NXPMCUX_FRDM_MCXN947_)
#include <sys/sysdepend/nxp_mcux/frdm_mcxn947/machine.h>

#elif defined(_XMCMTB_EVK_XMC7200_)
#include <sys/sysdepend/xmc_mtb/evk_xmc7200/machine.h>

#endif

/* ===== C compiler dependencies definitions ============================= */

#ifdef __GNUC__

#define Inline static __inline__
#define Asm __asm__ volatile
#define Noinit(decl) decl __attribute__((section(".noinit")))
#define	Section(decl,name) decl __attribute__((section(#name)))
#define WEAK_FUNC __attribute__((weak))

#define _VECTOR_ENTRY(name) .word name
#define _WEAK_ENTRY(name) .weak name

#endif /* __GNUC__ */

#endif /* _MTKBSP_SYS_MACHINE_H_ */
