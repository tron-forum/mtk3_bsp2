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

#ifndef	__DEV_I2C_HAL_XMC7200_H__
#define	__DEV_I2C_HAL_XMC7200_H__

void i2c_hal_do_int( unsigned int unit );
int i2c_hal_check_busy( unsigned int unit);
void i2c_hal_master_write(unsigned int unit, unsigned int sadr, unsigned int sdat_num, unsigned char *sbuf, unsigned int stop);
void i2c_hal_master_read(unsigned int unit, unsigned int sadr, unsigned int rdat_num, unsigned char *rbuf);
void i2c_hal_clock_init(unsigned int unit);
int i2c_hal_init(unsigned int unit);

#endif		/* __DEV_I2C_HAL_XMC7200_H__ */
