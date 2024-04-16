/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-24 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (EK-RA8M1)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H_
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H_

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- EK-RA8M1 (CPU: R7FA8M1AHECBD) definition ----- */
#define MTKBSP_RAFSP			1	/* Target system   : RA FSP */
#define MTKBSP_EK_RA8M1			1	/* Target Board    : EK-RA8M1 */

#define MTKBSP_CPU_RA			1	/* Target CPU type : RA group */
#define MTKBSP_CPU_RA8			1	/* Target CPU type : RA8 series */
#define MTKBSP_CPU_RA8M1		1	/* Target CPU      : RA8M1 */

#define MTKBSP_CPU_CORE_ARMV8M		1	/* Target CPU-Core type : ARMv8-M */
#define MTKBSP_CPU_CORE_ARMV81M		1	/* Target CPU-Core type : ARMv8.1-M */
#define MTKBSP_CPU_CORE_ACM85		1	/* Target CPU-Core      : ARM Cortex-M85 */

#define KNL_SYSDEP_PATH	sysdepend/ra_fsp	/* Kernel sysdepend path */

#define TARGET_DIR	ra_fsp/ek_ra8m1	/* Sysdepend-Directory name */
#define TARGET_GRP_DIR	ra_fsp		/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR	ra8m1		/* Sysdepend-CPU-Directory name */

/*
 **** CPU Core depeneded profile (ARMv8M)
 */
#include <sys/sysdepend/ra_fsp/cpu/core/armv8m/machine.h>

#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H_ */
