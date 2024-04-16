/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2024 by Ken Sakamura.
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
 *	Machine type definition (LPC55S69-EVK)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H_
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H_

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- LPC55S69-EVK (CPU: LPC55S69) definition ----- */
#define MTKBSP_MPCXPRESSO		1	/* Target system   : MPCXpresso */
#define MTKBSP_EVK_LPC55S69		1	/* Target Board    : LPC55S69-EVK */

#define MTKBSP_CPU_LPC55		1	/* Target CPU type : LPC55 group */
#define MTKBSP_CPU_LPC55S69		1	/* Target CPU      : LPC55S69 */

#define MTKBSP_CPU_CORE_ARMV8M		1	/* Target CPU-Core type : ARMv8-M */
#define MTKBSP_CPU_CORE_ACM33		1	/* Target CPU-Core      : ARM Cortex-M33 */

#define KNL_SYSDEP_PATH	sysdepend/nxp_mcux		/* Kernel sysdepend path */

#define TARGET_DIR	nxp_mcux/evk_lpc55s69		/* Sysdepend-Directory name */
#define TARGET_GRP_DIR	nxp_mcux 			/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR	lpc55s69			/* Sysdepend-CPU-Directory name */

/*
 **** CPU Core depeneded profile (ARMv8M)
 */
#include <sys/sysdepend/nxp_mcux/cpu/core/armv8m/machine.h>

#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H_ */
