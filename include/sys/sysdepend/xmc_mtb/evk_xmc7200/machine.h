/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (XMC7200D-E272K8384 depended)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H__
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- KIT_XMC72_EVK XMC7200 (CPU: XMC7200D-E272K8384) definition ----- */
#define MTKBSP_MODUSTOOLBOX		1		/* Target system : ModusToolBox */
#define MTKBSP_EVK_XMC7200		1		/* Target Board : XMC7200D-E272K8384 */

#define MTKBSP_CPU_XMC7200		1		/* Target CPU : XMC7200 series */
#define MTKBSP_CPU_XMC7200D		1		/* Target CPU : XMC7200D */
#define MTKBSP_CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define MTKBSP_CPU_CORE_ACM7		1		/* Target CPU-Core : ARM Cortex-M7 */

#define KNL_SYSDEP_PATH	sysdepend/xmc_mtb			/* Kernel sysdepend path */

#define TARGET_DIR		xmc_mtb/evk_xmc7200	/* Sysdepend-Directory name */
#define TARGET_GRP_DIR		xmc_mtb 		/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR		xmc7200			/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (XMC7200D-E272K8384)
 */
#include <sys/sysdepend/xmc_mtb/cpu/core/armv7m/machine.h>

/*
 *	NOT YET SUPPORT DEVICE
 */
#undef USE_PTMR


#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H__ */
