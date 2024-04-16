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
#include <sys/machine.h>
#include <config_bsp/stm32_cube/config_bsp.h>

#ifdef MTKBSP_STM32CUBE
#if DEVCNF_USE_HAL_ADC

#include <stdlib.h>

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/stm32_cube/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "hal_adc_cnf.h"

/*
 *	hal_adc.c
 *	A/DC device driver (STM32Cube FW)
*/

/*---------------------------------------------------------------------*/
/*A/DC Device driver Control block
 */
typedef struct {
	ADC_HandleTypeDef	*hadc;		// HAL handle
	ID			devid;		// Device ID
	UW			unit;		// Unit no
	UINT			omode;		// Open mode
	ID			tskid;		// Wait Task ID
	ER			err;		// Error code that occurred during interrupt processing
	ID			evtmbfid;	// MBF ID for event notification

	UW			val;		// A/DC converted data
} T_HAL_ADC_DCB;

/* Interrupt detection flag */
LOCAL ID	id_flgid;
LOCAL T_CFLG	id_flg	= {
			.flgatr		= TA_TFIFO | TA_WMUL,
			.iflgptn	= 0,
};

#if TK_SUPPORT_MEMLIB
LOCAL T_HAL_ADC_DCB	*dev_adc_cb[DEV_HAL_ADC_UNITNM];
#define		get_dcb_ptr(unit)	(dev_adc_cb[unit])
#else
LOCAL T_HAL_ADC_DCB	dev_adc_cb[DEV_HAL_ADC_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_ADC_cb[unit])
#endif

IMPORT ER dev_adc_setch(ADC_HandleTypeDef *hadc, UW unit, W start);

/*---------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_HAL_ADC_DCB *p_dcb, T_DEVREQ *req)
{
	return E_PAR;
}

LOCAL ER write_atr(T_HAL_ADC_DCB *p_dcb, T_DEVREQ *req)
{
	return E_PAR;
}

/*---------------------------------------------------------------------*/
/* Device-specific data control
 */

/* HAL Callback functions */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	T_HAL_ADC_DCB	*p_dcb;
	UINT	i;

	ENTER_TASK_INDEPENDENT

	for(i = 0; i < DEV_HAL_ADC_UNITNM; i++) {
		p_dcb = get_dcb_ptr(i);
		if(p_dcb->hadc == hadc) {
			p_dcb->err = E_OK;
			tk_set_flg(id_flgid, 1<< p_dcb->unit);
			break;
		}
	}

	LEAVE_TASK_INDEPENDENT
}

LOCAL ER read_data(T_HAL_ADC_DCB *p_dcb, T_DEVREQ *req)
{
	uint32_t		value;
	HAL_StatusTypeDef	hal_sts;
	UINT			wflgptn, rflgptn;
	ER			err;

	if(req->size == 0) {
		req->asize = 1;
		return E_OK;
	}
	// Configure for A/DC
	err = dev_adc_setch(p_dcb->hadc, p_dcb->unit, req->start);
	if(err != E_OK) return err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);
	hal_sts = HAL_ADC_Start_IT(p_dcb->hadc);
	if(hal_sts != HAL_OK) return E_BUSY;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_ADC_TMOUT);
	if(err >= E_OK) {
		err  = p_dcb->err;
		if(err >= E_OK) {
			value = HAL_ADC_GetValue(p_dcb->hadc);
			*(UW*)(req->buf) = (UW)value;
			req->asize= 1;
		}
	}

	return err;
}

LOCAL ER write_data(T_HAL_ADC_DCB *p_dcb, T_DEVREQ *req)
{
	return E_RONLY;
}

/*----------------------------------------------------------------------
 * mSDI I/F function
 */
/*
 * Open device
 */
LOCAL ER dev_adc_openfn( ID devid, UINT omode, T_MSDI *msdi)
{
	T_HAL_ADC_DCB	*p_dcb;

	p_dcb = (T_HAL_ADC_DCB*)(msdi->dmsdi.exinf);
	if(p_dcb->hadc == NULL) return E_IO;

	p_dcb->omode = omode;
	return E_OK;
}

/*
 * Close Device
 */
LOCAL ER dev_adc_closefn( ID devid, UINT option, T_MSDI *msdi)
{
	return E_OK;
}

/*
 * Read Device
 */
LOCAL ER dev_adc_readfn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_HAL_ADC_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_HAL_ADC_DCB*)(p_msdi->dmsdi.exinf);
	if(p_dcb->hadc == NULL) return E_IO;

	if(req->start >= 0) {
		err = read_data( p_dcb, req);	// Device specific data
	} else {
		err = read_atr( p_dcb, req);	// Device attribute data
	}
	return err;
}

/*
 * Write Device
 */
LOCAL ER dev_adc_writefn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_HAL_ADC_DCB	*p_dcb;
	ER		rtn;

	p_dcb = (T_HAL_ADC_DCB*)(p_msdi->dmsdi.exinf);
	if(p_dcb->hadc == NULL) return E_IO;

	if(req->start >= 0) {
		rtn = write_data( p_dcb, req);	// Device specific data
	} else {
		rtn = write_atr( p_dcb, req);	// Device attribute data
	}
	return rtn;
}

/*
 * Event Device
 */
LOCAL ER dev_adc_eventfn( INT evttyp, void *evtinf, T_MSDI *msdi)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */
EXPORT ER dev_init_hal_adc( UW unit, ADC_HandleTypeDef *hadc )
{
	T_HAL_ADC_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_HAL_ADC_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_HAL_ADC_DCB*)Kmalloc(sizeof(T_HAL_ADC_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_adc_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_adc_cb[unit];
#endif

	id_flgid = tk_cre_flg(&id_flg);
	if(id_flgid <= E_OK) {
		err = (ER)id_flgid;
		goto err_1;
	}

	/* Device registration information */
	dmsdi.exinf	= p_dcb;
	dmsdi.drvatr	= 0;			/* Driver attributes */
	dmsdi.devatr	= TDK_UNDEF;		/* Device attributes */
	dmsdi.nsub	= 0;			/* Number of sub units */
	dmsdi.blksz	= 1;			/* Unique data block size (-1 = unknown) */
	dmsdi.openfn	= dev_adc_openfn;
	dmsdi.closefn	= dev_adc_closefn;
	dmsdi.readfn	= dev_adc_readfn;
	dmsdi.writefn	= dev_adc_writefn;
	dmsdi.eventfn	= dev_adc_eventfn;
	
	knl_strcpy( (char*)dmsdi.devnm, DEVNAME_HAL_ADC);
	i = knl_strlen(DEVNAME_HAL_ADC);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_1;

	p_dcb->hadc	= hadc;
	p_dcb->devid	= p_msdi->devid;
	p_dcb->unit	= unit;
	p_dcb->evtmbfid	= idev.evtmbfid;

	return E_OK;

err_1:
#if TK_SUPPORT_MEMLIB
	Kfree(p_dcb);
#endif
	return err;
}

#endif		/* DEVCNF_USE_HAL_ADC */
#endif		/* MTKBSP_STM32CUBE */