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
 *	device.h
 *
 *	Device driver definition
 */

#ifndef _MTKBSP_TK_DEVICE_H_
#define _MTKBSP_TK_DEVICE_H_

#define DEVICE_PATH_(a)		#a
#define DEVICE_PATH(a)		DEVICE_PATH_(a)
#define DEVICE_SYSDEP()		DEVICE_PATH(sysdepend/TARGET_GRP_DIR/device/device.h)
#include DEVICE_SYSDEP()

#endif /* _MTKBSP_TK_DEVICE_H_ */