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

#ifndef	_DEV_HAL_I3C_I2C_H_
#define	_DEV_HAL_I3C_I2C_H_
/*
 *	hal_i3c_i2c.h
 *	I3C device driver (RA FSP)
*/
/*----------------------------------------------------------------------
 * I3C Device
 */
#define DEV_HAL_I3C1	0

/*----------------------------------------------------------------------
 * Attribute data
 */
#define TDN_HAL_I3C_I2C_MODE	(-100)	// I3C Mode
#define TDN_HAL_I3C_I2C_TADR	(-101)	// Target Address
#define TDN_HAL_I3C_I2C_MAX		(-101)

#define HAL_I3C_I2C_MODE_CNT	(0)	// I3C Mode: Controller mode	
#define HAL_I3C_I2C_MODE_TAR	(1)	// I3C Mode: Target mode

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */

IMPORT ER dev_init_hal_i3c_i2c( UW unit, i3c_instance_ctrl_t *hi3c, const i3c_cfg_t *ci3c);
/*----------------------------------------------------------------------
 * I3C register access support function
 */
IMPORT ER hal_i3c_i2c_read_reg(ID dd, UW sadr, UW radr, UB *data);
IMPORT ER hal_i3c_i2c_write_reg(ID dd, UW sadr, UW radr, UB data);

#endif	/* _DEV_HAL_I3C_I2C_H_ */