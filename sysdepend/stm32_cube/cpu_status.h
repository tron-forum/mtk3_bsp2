/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpu_status.h
 *	CPU-Dependent Task Start Processing
 */

#ifndef _MTKBSP_SYSDEPEND_CPUSTATUS_
#define _MTKBSP_SYSDEPEND_CPUSTATUS_

#if MTKBSP_CPU_CORE_ARMV7M
#include <sysdepend/stm32_cube/cpu/core/armv7m/cpu_status.h>

#elif MTKBSP_CPU_CORE_ARMV8M
#include <sysdepend/stm32_cube/cpu/core/armv8m/cpu_status.h>
#endif

#endif /* _MTKBSP_SYSDEPEND_CPUSTATUS_ */