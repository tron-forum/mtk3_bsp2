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

#include <sys/machine.h>
#ifdef MTKBSP_CPU_XMC7200

#include <tk/tkernel.h>
#include "../hal_adc.h"
#include "../hal_adc_cnf.h"
#include "../../../include/dev_def.h"
#include "adc_hal_xmc7200.h"
#include "adc_sysdep.h"
#include "adc_cnf_sysdep.h"
#if DEVCNF_USE_HAL_ADC
/*
 *	adc_xmc7200.c
 *	A/D converter device driver
 *	System dependent processing for XMC7200
 */

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	volatile ID	wait_tskid;
} ll_devcb[DEV_HAL_ADC_UNITNM] = {
	{0},
	{0},
	{0},
};

/*----------------------------------------------------------------------
 * Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	if(ll_devcb[1].wait_tskid) {
		tk_wup_tsk(ll_devcb[1].wait_tskid);
	}

	adc_hal_clear_interrupt();	// Clear interrupt flag.
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( UINT unit, INT ch, INT size, UW *buf )
{
	ER	err;
	UINT	i;

	if((ch >= ADC_CH_NUM) || (size > ADC_MAX_SQ) || ((ch+size) > ADC_CH_NUM)) return (UW)E_PAR;

	ll_devcb[unit].wait_tskid = tk_get_tid();

	tk_can_wup(TSK_SELF);
	adc_hal_start_convert();	// Start Covert
	err = tk_slp_tsk(DEVCNF_HAL_ADC_TMOUT);
	ll_devcb[unit].wait_tskid = 0;
	
	if (err >= E_OK) {
		for (i = ch; i < (ch + size); i++){
			buf[i - ch] = adc_hal_get_result(i);
		}
		err = size;
	}

	return err;
}


/*----------------------------------------------------------------------
 * A/DC open
 */
LOCAL ER adc_open(UW unit)
{
	ER	err = E_OK;

	if (adc_hal_open() != 0) {
		return E_SYS;
	}
	EnableInt(INTNO_INTSAR1_CH16, DEVCNF_SAR1_INTPRI);

	/* Enable ADC */
	ll_devcb[unit].wait_tskid = tk_get_tid();
	adc_hal_start_convert();

	err = tk_slp_tsk(DEVCNF_HAL_ADC_TMOUT);
	if(err < E_OK) err = E_TMOUT;
	ll_devcb[unit].wait_tskid = 0;

	return err;
}

/*----------------------------------------------------------------------
 * A/DC close
 */
LOCAL void adc_close(UW unit)
{
	DisableInt(INTNO_INTSAR1_CH16);

	/* Disable ADC */
	adc_hal_close();
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	W	rtn	= (W)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open A/DC */
		rtn = (W)adc_open(unit);
		break;

	case LLD_ADC_CLOSE:	/* Close A/DC */
		adc_close(unit);
		break;
	
	case LLD_ADC_READ:	/* Read A/DC data */
		rtn = adc_convert(unit, p1, p2, pp);
		break;
	
	case LLD_ADC_RSIZE:	/* Get read data size */
		rtn = ADC_CH_NUM - p1;
		if(rtn < 0 ) rtn = 0;
		break;
	}
	
	return rtn;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_adc_llinit( T_ADC_DCB *p_dcb)
{
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= adc_inthdr
	};
	ER	err;
	
	if (p_dcb->unit != 1) {
		return E_NOSPT;
	}

#if DEVCONF_ADC_INIT_MCLK
	/* Initialize clock source. */
	adc_hal_init_clock();
#endif
	/* Initialize A/DC module. */
	adc_hal_init();

	/* Interrupt handler definition */
	err = tk_def_int(INTNO_INTSAR1_CH16, &dint);

	return err;
}

#endif		/* DEVCNF_USE_HAL_ADC */
#endif		/* MTKBSP_CPU_XMC7200 */
