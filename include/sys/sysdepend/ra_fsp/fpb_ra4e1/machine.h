/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (FPB-RA4E1)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H_
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H_

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- FPB-RA4E1 (CPU: R7FA4E10D2CFM) definition ----- */
#define MTKBSP_RAFSP			1	/* Target system   : RA FSP */
#define MTKBSP_FPB_RA4E1		1	/* Target Board    : FPB-RA4E1 */

#define MTKBSP_CPU_RA			1	/* Target CPU type : RA group */
#define MTKBSP_CPU_RA4			1	/* Target CPU type : RA4 series */
#define MTKBSP_CPU_RA4E1		1	/* Target CPU      : RA4E1 */

#define MTKBSP_CPU_CORE_ARMV8M		1	/* Target CPU-Core type : ARMv8-M */
#define MTKBSP_CPU_CORE_ACM33		1	/* Target CPU-Core      : ARM Cortex-M33 */

#define KNL_SYSDEP_PATH	sysdepend/ra_fsp	/* Kernel sysdepend path */

#define TARGET_DIR	ra_fsp/fpb_ra4e1	/* Sysdepend-Directory name */
#define TARGET_GRP_DIR	ra_fsp			/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR	ra4e1			/* Sysdepend-CPU-Directory name */

/*
 **** CPU Core depended profile (ARMv8M)
 */
#include <sys/sysdepend/ra_fsp/cpu/core/armv8m/machine.h>

#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H_ */
