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
 *	cpudef.h
 *
 *	CPU dependent definition (RA FSP)
 */

#ifndef _MTKBSP_TK_CPUDEF_DEPEND_H_
#define _MTKBSP_TK_CPUDEF_DEPEND_H_

#if MTKBSP_CPU_CORE_ARMV7M
#include <tk/sysdepend/ra_fsp/cpu/core/armv7m/cpudef.h>

#elif MTKBSP_CPU_CORE_ARMV8M
#include <tk/sysdepend/ra_fsp/cpu/core/armv8m/cpudef.h>
#endif

#endif /* _MTKBSP_TK_CPUDEF_DEPEND_H_ */
