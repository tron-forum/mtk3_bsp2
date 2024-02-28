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
 *	hal_lpi2c_cnf.h 
 *	I2C device driver (NXP MCUXPresso)
 *		Device configuration file
 */
#ifndef	_DEV_HAL_LPI2C_CNF_H_
#define	_DEV_HAL_LPI2C_CNF_H_

#define DEVNAME_HAL_LPI2C	"hiic"
#define DEV_HAL_LPI2C_TMOUT	(500)

#define DEV_HAL_LPI2C_UNITNM	(2)	// Number of I2C units (max 26)

#endif	/* _DEV_HAL_LPI2C_CNF_H_ */
