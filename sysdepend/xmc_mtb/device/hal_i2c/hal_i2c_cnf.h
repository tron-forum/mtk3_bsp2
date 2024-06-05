/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	i2c_cnf.h 
 *	I2C device driver
 *		Device configuration file
 */
#ifndef	_DEV_HAL_I2C_CNF_H_
#define	_DEV_HAL_I2C_CNF_H_


#define	DEVNAME_HAL_I2C		"hiic"			// Device name ("hiic")

#define	DEVCNF_I2C_MAX_SDATSZ	100		// Maximum transmission data size
#define	DEVCNF_I2C_MAX_RDATSZ	100		// Maximum received data size


#endif		/* _DEV_HAL_I2C_CNF_H_ */
