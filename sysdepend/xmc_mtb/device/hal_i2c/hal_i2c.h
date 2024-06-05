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

#ifndef	_DEV_HAL_I2C_H_
#define	_DEV_HAL_I2C_H_
/*
 *	hal_i2c.h
 *	I2C device driver
 *	Driver local definition
 */

#include <mtkernel/device/common/drvif/msdrvif.h>
#include "../include/dev_i2c.h"
#include "hal_i2c_cnf.h"

/*----------------------------------------------------------------------
 * I2C Device
 */
#define DEV_HAL_I2C9		9


/*---------------------------------------------------------------------*/
/* Device driver Control block
 */
typedef struct {
	UW	unit;		/* Unit No. */
	UINT	omode;		/* Open mode */

	ID	evtmbfid;			/* MBF ID for event notification */
} T_I2C_DCB;

/*---------------------------------------------------------------------*/
/* Device driver low-level interface
 * 	These functions are defined in the system dependent part.
 */
IMPORT W dev_i2c_llctl( UW, INT, UW, UW, UW*);		/* Low-Level device control */
IMPORT ER dev_i2c_llinit( T_I2C_DCB *p_dcb);	/* Low-level device initialization */

typedef enum {
	LLD_I2C_OPEN,
	LLD_I2C_CLOSE,
	LLD_I2C_READ,
	LLD_I2C_WRITE,
	LLD_I2C_EXEC,
} T_LLD_I2C_CMD;

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */

IMPORT ER dev_init_hal_i2c( UW unit );

/*----------------------------------------------------------------------
 * I2C register access support function
 */
EXPORT ER i2c_read_reg(ID dd, UW sadr, UW radr, UB *data);
EXPORT ER i2c_write_reg(ID dd, UW sadr, UW radr, UB data);


#endif		/* _DEV_HAL_I2C_H_ */
