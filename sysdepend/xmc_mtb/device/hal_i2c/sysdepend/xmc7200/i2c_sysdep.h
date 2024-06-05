/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	i2c_sysdep.h
 *	I2C device driver
 *	System-dependent definition for XMC7200
 */

#ifndef	__DEV_I2C_XMC7200_H__
#define	__DEV_I2C_XMC7200_H__

#define	DEV_I2C_UNITNM		(11)	/* Number of devive units */
#define	DEV_I2C_9		(9)	/* SCB9 */

/* 
 * I2C interrupt number
  */
#define	INTNO_SCB9		121		// SCB9 interrupt no

#endif		/* __DEV_I2C_XMC7200_H__ */
