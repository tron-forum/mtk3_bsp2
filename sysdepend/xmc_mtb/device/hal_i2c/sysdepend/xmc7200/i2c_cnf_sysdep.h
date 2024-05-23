/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */


/*
 *	i2c_cnf_sysdep.h 
 *	I2C device configuration file
 *		for XMC7200
 */
#ifndef	__DEV_I2C_CNF_XMC7200_H__
#define	__DEV_I2C_CNF_XMC7200_H__

/* Device clock initialization */
#define DEVCNF_I2C_INIT_MCLK		TRUE		// Initialize module clock

/* I2C master desired data rate in Hz */
#define DEVCNF_I2C_DATA_RATE_HZ    (100000U)

// Interrupt priority
#define	DEVCNF_I2C9_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C9_TMO		1000


#endif		/* __DEV_I2C_CNF_XMC7200_H__ */
