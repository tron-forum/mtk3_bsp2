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
 *	sysdef.h
 *
 *	System dependencies definition.
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_H__
#define __SYS_SYSDEF_H__

/* System dependencies */
#define SYSDEF_PATH_(a)		#a
#define SYSDEF_PATH(a)		SYSDEF_PATH_(a)
#define SYSDEF_SYSDEP()		SYSDEF_PATH(sys/sysdepend/TARGET_DIR/sysdef.h)
#include SYSDEF_SYSDEP()

#ifndef _in_asm_source_
#if USE_DEBUG_SYSMEMINFO
IMPORT void *knl_sysmem_top;
IMPORT void *knl_sysmem_end;
#endif
#endif

#endif /* __SYS_SYSDEF_H__ */
