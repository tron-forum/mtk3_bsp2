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
 *	System dependencies definition (STM32Cule NUCLEO STM32H533)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_H_

/* TrustZone defined */
/* This implementation does not support TrustZone */
#define TRUSTZONE_ENABLE	0	// TrustZone Disabled

/* CPU-dependent definition */
#include <sys/sysdepend/stm32_cube/cpu/stm32h5/sysdef.h>

#endif /* _MTKBSP_TK_SYSDEF_DEPEND_H_ */
