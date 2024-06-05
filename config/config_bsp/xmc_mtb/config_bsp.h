/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	config_bsp.h
 *	BSP Configuration Definition (XMC MTB)
 */

#ifndef	_MTKBSP_BSP_CONFIG_DEVENV_H_
#define	_MTKBSP_BSP_CONFIG_DEVENV_H_

/* ------------------------------------------------------------------------ */
/*
 * Static allocation of system memory
 *     Enabling this setting statically allocates system memory space as variables.
 */
#define USE_STATIC_SYS_MEM	(0)		// 1:Valid   0:invalid
#define SYSTEM_MEM_SIZE		(15*1024)	// Memory size to statically allocate.

/* ------------------------------------------------------------------------ */
/*
 *  System memory area information (For debugging)
 */
#define USE_DEBUG_SYSMEMINFO   (1)		// 1:Valid   0:invalid

/* ------------------------------------------------------------------------ */
/*
 *  Stack pointer monitoring function
 */
#define USE_SPMON		(1)		// 1:Valid   0:invalid

/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_IIC		0	// I2C communication device (Use IIC )
#define DEVCNF_USE_HAL_ADC		0	// A/D conversion device

#endif	/* _MTKBSP_BSP_CONFIG_DEVENV_H_ */
