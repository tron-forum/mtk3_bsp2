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
 *	profile.h
 *
 *	Service Profile (XMC7200D-E272K8384 depended)
 */

#ifndef _MTKBSP_SYS_DEPEND_PROFILE_H__
#define _MTKBSP_SYS_DEPEND_PROFILE_H__

/*
 **** CPU-depeneded profile (XMC7200)
 */
#include <sys/sysdepend/xmc_mtb/cpu/core/armv7m/profile.h>

/*
 **** Target-depeneded profile (XMC7200D-E272K8384)
 */

/*
 * Power management
 */
#define TK_SUPPORT_LOWPOWER	FALSE		/* Support of power management */



#endif /* _MTKBSP_SYS_DEPEND_PROFILE_H__ */
