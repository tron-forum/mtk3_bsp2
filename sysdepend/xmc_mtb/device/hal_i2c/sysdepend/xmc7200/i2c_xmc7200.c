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

#include <sys/machine.h>
#include <config_bsp/xmc_mtb/config_bsp.h>

#ifdef MTKBSP_CPU_XMC7200
#if DEVCNF_USE_HAL_IIC


#include <tk/tkernel.h>
#include "../../hal_i2c.h"
#include "i2c_sysdep.h"
#include "i2c_hal_xmc7200.h"
#include "i2c_cnf_sysdep.h"

/*
 *	i2c_xmc7200.c
  *	I2C device driver
 *	System-dependent definition for XMC7200
 */

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
	UINT	intno;		// Interrupt number
	PRI	intpri;		// Interrupt priority
	TMO	timout;		// I2C timeout time
} ll_devdat[DEV_I2C_UNITNM] = {
	{0},	/* I2C0 */
	{0},	/* I2C1 */
	{0},	/* I2C2 */
	{0},	/* I2C3 */
	{0},	/* I2C4 */
	{0},	/* I2C5 */
	{0},	/* I2C6 */
	{0},	/* I2C7 */
	{0},	/* I2C8 */
	{	/* I2C9 */
		.intno		= INTNO_SCB9,
		.intpri		= DEVCNF_I2C9_INTPRI,
		.timout		= DEVCNF_I2C9_TMO,
	},
	{0},	/* I2C10 */
};

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	ID	wait_tskid;	/* Wait Task ID */
} T_I2C_LLDCB;
LOCAL T_I2C_LLDCB	ll_devcb[DEV_I2C_UNITNM];


/*-------------------------------------------------------
 * Interrupt handler
 */
/*-------------------------------------------------------
 * Event interrupt handler
 */
LOCAL void i2c_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;
/*
	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( ll_devdat[unit].intno == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
*/
	unit = DEV_I2C_9;
	p_cb = &ll_devcb[unit];
	
	i2c_hal_do_int(unit);

	if (i2c_hal_check_busy(unit) == 0) {
		if (p_cb->wait_tskid) {
			tk_wup_tsk(p_cb->wait_tskid);
			p_cb->wait_tskid = 0;
		}
	}
}

/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans(UW unit, UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	ER		err;
	UW		stop = ((rdat_num == 0) || (rbuf == NULL));

	ll_devcb[unit].wait_tskid	= 0;

	i2c_hal_master_write(unit, sadr, sdat_num, sbuf, stop);

	ll_devcb[unit].wait_tskid = tk_get_tid();
	err = tk_slp_tsk(ll_devdat[unit].timout);
	if ( err < E_OK ) return err;
	
	if (!stop) {
		i2c_hal_master_read(unit, sadr, rdat_num, rbuf);

		ll_devcb[unit].wait_tskid = tk_get_tid();
		err = tk_slp_tsk(ll_devdat[unit].timout);
		if ( err < E_OK ) return err;
	}

	return err;
}


/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_i2c_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	T_I2C_EXEC	*p_ex;
	ER		err	= E_OK;

	switch(cmd) {
	case LLD_I2C_OPEN:
		/* I2C interrupt enable */
		EnableInt(ll_devdat[unit].intno, ll_devdat[unit].intpri);
		break;

	case LLD_I2C_CLOSE:
		/* I2C interrupt disable */
		DisableInt(ll_devdat[unit].intno);
		break;

	case LLD_I2C_READ:
		err = i2c_trans( unit, p1, 0, p2, NULL, (UB*)pp );
		if(err >= E_OK) err = p2;
		break;

	case LLD_I2C_WRITE:
		err = i2c_trans( unit, p1, p2, 0, (UB*)pp, NULL );
		if(err >= E_OK) err = p2;
		break;

	case LLD_I2C_EXEC:
		p_ex = (T_I2C_EXEC*)pp;
		err = i2c_trans( unit, p_ex->sadr, p_ex->snd_size, p_ex->rcv_size, p_ex->snd_data, p_ex->rcv_data );
		if(err >= E_OK) err = p_ex->snd_size + p_ex->rcv_size;
		break;
	}
	
	return (W)err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_i2c_llinit( T_I2C_DCB *p_dcb)
{
	T_DINT	dint;

	UW	unit;
	UW	intno;
	ER	err;

	unit = p_dcb->unit;

#if DEVCNF_I2C_INIT_MCLK
	i2c_hal_clock_init(unit);
#endif

	if(unit == DEV_I2C_9) {
		err = i2c_hal_init(unit);
		if(err < 0) {
			return E_SYS;
		}
	}

	/* Interrupt handler definition */
	intno		= ll_devdat[unit].intno;
	dint.intatr	= TA_HLNG;
	dint.inthdr	= i2c_inthdr;
	
	err = tk_def_int(intno, &dint);		// I2C interrupt handler
	if(err < E_OK) return err;

	return err;
}

#endif		/* DEVCNF_USE_HAL_IIC */
#endif		/* MTKBSP_CPU_XMC7200 */
