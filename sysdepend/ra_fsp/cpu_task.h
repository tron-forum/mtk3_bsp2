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
 *	cpu_task.h
 *	CPU-Dependent Task Start Processing
 */

#ifndef _MTKBSP_SYSDEPEND_CPUTASK_
#define _MTKBSP_SYSDEPEND_CPUTASK_

#if MTKBSP_CPU_CORE_ARMV7M
#include <sysdepend/ra_fsp/cpu/core/armv7m/cpu_task.h>

#elif MTKBSP_CPU_CORE_ARMV8M
#include <sysdepend/ra_fsp/cpu/core/armv8m/cpu_task.h>
#endif

#endif /* _MTKBSP_SYSDEPEND_CPUTASK_ */