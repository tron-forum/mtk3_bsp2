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
 *	machine.h
 *
 *	Machine type definition (Arduino UNO R4)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H_
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H_

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- Arduino UNO R4 (CPU: R7FA4M1AB3CFM) definition ----- */
#define MTKBSP_RAFSP			1	/* Target system   : RA FSP */
#define MTKBSP_ARDUINO_UNOR4		1	/* Target Board    : Arduino UNO R4 */

#define MTKBSP_CPU_RA			1	/* Target CPU type : RA group */
#define MTKBSP_CPU_RA4			1	/* Target CPU type : RA4 series */
#define MTKBSP_CPU_RA4M1		1	/* Target CPU      : RA4M1 */

#define MTKBSP_CPU_CORE_ARMV7M		1	/* Target CPU-Core type : ARMv7-M */
#define MTKBSP_CPU_CORE_ACM4		1	/* Target CPU-Core      : ARM Cortex-M4 */

#define KNL_SYSDEP_PATH	sysdepend/ra_fsp	/* Kernel sysdepend path */

#define TARGET_DIR	ra_fsp/arduino_unor4	/* Sysdepend-Directory name */
#define TARGET_GRP_DIR	ra_fsp			/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR	ra4m1			/* Sysdepend-CPU-Directory name */

/*
 **** CPU Core depended profile (ARMv7M)
 */
#include <sys/sysdepend/ra_fsp/cpu/core/armv7m/machine.h>

#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H_ */
