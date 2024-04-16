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
 *	Machine type definition (ARMv8-M)
 */

#ifndef _MTKBSP_SYS_MACHINE_CORE_H_
#define _MTKBSP_SYS_MACHINE_CORE_H_

/*
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * INT_BITWIDTH		Integer size (bit)
 * BIGENDIAN		1 if big endian 
 */

/* ----- ARMv8-M definition ----- */

#define ALLOW_MISALIGN		0
#define INT_BITWIDTH		32
#define BIGENDIAN		0	/* Default (Little Endian) */

#endif /* _MTKBSP_SYS_MACHINE_CORE_H_ */
