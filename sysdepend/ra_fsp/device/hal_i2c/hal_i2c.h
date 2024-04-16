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

#ifndef	_DEV_HAL_I2C_H_
#define	_DEV_HAL_I2C_H_
/*
 *	hal_i2c.h
 *	I2C device driver (RA FSP)
*/
/*----------------------------------------------------------------------
 * I2C Device
 */
#define DEV_HAL_I2C1	0

/*----------------------------------------------------------------------
 * Attribute data
 */
#define TDN_HAL_I2C_MODE	(-100)	// I2C Mode
#define TDN_HAL_I2C_TADR	(-101)	// Target Address
#define TDN_HAL_I2C_MAX		(-101)

#define HAL_I2C_MODE_CNT	(0)	// I2C Mode: Controller mode	
#define HAL_I2C_MODE_TAR	(1)	// I2C Mode: Target mode

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */

IMPORT ER dev_init_hal_i2c( UW unit, i2c_master_ctrl_t *hi2c, const i2c_master_cfg_t *ci2c );

/*----------------------------------------------------------------------
 * I2C register access support function
 */
EXPORT ER hal_i2c_read_reg(ID dd, UW sadr, UW radr, UB *data);
EXPORT ER hal_i2c_write_reg(ID dd, UW sadr, UW radr, UB data);

#endif	/* _DEV_HAL_I2C_H_ */