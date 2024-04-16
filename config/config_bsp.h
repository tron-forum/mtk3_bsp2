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
  *	config_bsp.h
  *	BSP Configuration Definition
  */

#ifndef	_MTKBSP_BSP_CONFIG_H_
#define	_MTKBSP_BSP_CONFIG_H_

/* System dependencies */
#define BSPCNF_PATH_(a)		#a
#define BSPCNF_PATH(a)		BSPCNF_PATH_(a)
#define BSPCNF_SYSDEP()		BSPCNF_PATH(config_bsp/TARGET_GRP_DIR/config_bsp.h)
#include BSPCNF_SYSDEP()

#endif  /* _MTKBSP_BSP_CONFIG_H_ */
