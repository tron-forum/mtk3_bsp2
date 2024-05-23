/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h (XMC7200)
 *	System-Dependent local defined
 */

#ifndef _MTKBSP_CPU_SYSDEPEND_
#define _MTKBSP_CPU_SYSDEPEND_

#include "sysdepend/xmc_mtb/cpu/core/armv7m/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(void);
IMPORT void shutdown_clock(void);

#endif /* _MTKBSP_CPU_SYSDEPEND_ */
