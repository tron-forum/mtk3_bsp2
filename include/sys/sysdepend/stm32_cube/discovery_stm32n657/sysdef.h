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
 *	sysdef.h
 *
 *	System dependencies definition (STM32Cule STM32N6570-DK)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_H_

/* TrustZone defined */
#define TRUSTZONE_ENABLE	1	// TrustZone Enabled
#define TRUSTZONE_SECURE	1	// OS in Secure status 

/* CPU-dependent definition */
#include <sys/sysdepend/stm32_cube/cpu/stm32n6/sysdef.h>

#endif /* _MTKBSP_TK_SYSDEF_DEPEND_H_ */
