/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (STM32N6570-DK)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H_
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H_

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- STM32N6570-DK (CPU: STM32N657X0) definition ----- */
#define MTKBSP_STM32CUBE		1	/* Target system   : STM32Cube */
#define MTKBSP_DISCOVERY_STM32N657	1	/* Target Board    : STM32N6570-DK */

#define MTKBSP_CPU_STM32		1	/* Target CPU type : STM32 group */
#define MTKBSP_CPU_STM32N6		1	/* Target CPU type : STM32N6 series */
#define MTKBSP_CPU_STM32N657		1	/* Target CPU      : STM32N657 */

#define MTKBSP_CPU_CORE_ARMV8M		1	/* Target CPU-Core type : ARMv8-M */
#define MTKBSP_CPU_CORE_ACM55		1	/* Target CPU-Core      : ARM Cortex-M55 */

#define KNL_SYSDEP_PATH	sysdepend/stm32_cube			/* Kernel sysdepend path */

#define TARGET_DIR	stm32_cube/discovery_stm32n657	/* Sysdepend-Directory name */
#define TARGET_GRP_DIR	stm32_cube 			/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR	stm32n6				/* Sysdepend-CPU-Directory name */

/*
 **** CPU Core depended profile (ARMv8M)
 */
#include <sys/sysdepend/stm32_cube/cpu/core/armv8m/machine.h>

#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H_ */
