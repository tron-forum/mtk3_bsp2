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
 *	sys_timer.h
 *	Hardware-Dependent System Timer Processing
 */

#ifndef _MTKBSP_SYSDEPEND_SYSTIMER_
#define _MTKBSP_SYSDEPEND_SYSTIMER_

#if MTKBSP_CPU_CORE_ARMV7M
#include <sysdepend/ra_fsp/cpu/core/armv7m/sys_timer.h>

#elif MTKBSP_CPU_CORE_ARMV8M
#include <sysdepend/ra_fsp/cpu/core/armv8m/sys_timer.h>
#endif

#endif /* _MTKBSP_SYSDEPEND_SYSTIMER_ */