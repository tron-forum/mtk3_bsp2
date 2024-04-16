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
 *	cpudef.h
 *
 *	CPU dependent definition
 */

#ifndef _MTKBSP_TK_CPUDEF_H_
#define _MTKBSP_TK_CPUDEF_H_

/* ------------------------------------------------------------------------ */
/*
 * System dependencies  (CPU Interrupt control , I/O port access)
 */
#define CPUDEF_PATH_(a)		#a
#define CPUDEF_PATH(a)		CPUDEF_PATH_(a)
#define CPUDEF_SYSDEP()		CPUDEF_PATH(tk/sysdepend/TARGET_GRP_DIR/cpudef.h)
#include CPUDEF_SYSDEP()

#endif	/* _MTKBSP_TK_CPUDEF_H_ */
