/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/04.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <config_bsp/ra_fsp/config_bsp.h>

#ifdef MTKBSP_RAFSP
#if DEVCNF_USE_HAL_ADHB

#include <stdlib.h>

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/ra_fsp/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "hal_adhb_cnf.h"

/*
 *	hal_adhb.c
 *	A/DC device driver (RA FSP)
*/

/*---------------------------------------------------------------------*/
/*A/DC Device driver Control block
 */
typedef struct {
	adc_b_instance_ctrl_t	*hadc;		// ADC handle
	const adc_cfg_t		*cadc;		// ADC config
	const adc_b_scan_cfg_t	*sadc;		// ADC scan groups
	ID			devid;		// Device ID
	UW			unit;		// Unit no
	UINT			omode;		// Open mode
	ER			err;		// Error code that occurred during interrupt processing
	ID			evtmbfid;	// MBF ID for event notification
	UW			val;		// A/DC converted data
} T_HAL_ADHB_DCB;

/* Interrupt detection flag */
LOCAL ID	id_flgid;
LOCAL T_CFLG	id_flg	= {
			.flgatr		= TA_TFIFO | TA_WMUL,
			.iflgptn	= 0,
};

#if TK_SUPPORT_MEMLIB
LOCAL T_HAL_ADHB_DCB	*dev_adhb_cb[DEV_HAL_ADHB_UNITNM];
#define		get_dcb_ptr(unit)	(dev_adhb_cb[unit])
#else
LOCAL T_HAL_ADHB_DCB	dev_adhb_cb[DEV_HAL_ADHB_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_adhb_cb[unit])
#endif

/*---------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_HAL_ADHB_DCB *p_dcb, T_DEVREQ *req)
{
	return E_PAR;
}

LOCAL ER write_atr(T_HAL_ADHB_DCB *p_dcb, T_DEVREQ *req)
{
	return E_PAR;
}

/*---------------------------------------------------------------------*/
/* Device-specific data control
 */

/* HAL Callback functions */
LOCAL void HAL_ADHB_Callback(adc_callback_args_t *p_args)
{
	T_HAL_ADHB_DCB	*p_dcb;

	ENTER_TASK_INDEPENDENT

	p_dcb = (T_HAL_ADHB_DCB*)p_args->p_context;
	tk_set_flg(id_flgid, 1<< p_dcb->unit);

	switch(p_args->event) {
		case ADC_EVENT_SCAN_COMPLETE:
		case ADC_EVENT_CALIBRATION_COMPLETE:
			p_dcb->err = E_OK;
			break;
		default:
			p_dcb->err = E_IO;
			break;
	}

	LEAVE_TASK_INDEPENDENT
}

LOCAL ER read_data(T_HAL_ADHB_DCB *p_dcb, T_DEVREQ *req)
{
	uint16_t	val;
	UINT		wflgptn, rflgptn;
	ER		err;
	
	fsp_err_t	fsp_err;

	if(req->size == 0) {
		req->asize = 1;
		return E_OK;
	}

	/* Start the ADC scan */
	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	fsp_err = R_ADC_B_ScanStart(p_dcb->hadc);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_ADHB_TMOUT);
	if(err >= E_OK) {
		err = p_dcb->err;
		if(err >= E_OK) {
			/* Read the result */
			fsp_err = R_ADC_B_Read(p_dcb->hadc, req->start, &val);
			if(fsp_err == FSP_SUCCESS) {
				*(UW*)(req->buf) = val;
				req->asize= 1;
			} else {
				err = E_IO;
			}
		}
	}

	return err;
}

LOCAL ER write_data(T_HAL_ADHB_DCB *p_dcb, T_DEVREQ *req)
{
	return E_RONLY;
}

/*----------------------------------------------------------------------
 * mSDI I/F function
 */
/*
 * Open device
 */
LOCAL ER dev_adhb_openfn( ID devid, UINT omode, T_MSDI *p_msdi)
{
	T_HAL_ADHB_DCB	*p_dcb;
	UINT		wflgptn, rflgptn;
	ER		err = E_OK;

	fsp_err_t	fsp_err;

	p_dcb = (T_HAL_ADHB_DCB*)(p_msdi->dmsdi.exinf);
	if(p_dcb->hadc == NULL) return E_IO;

	p_dcb->omode = omode;

	/* Open/Initialize ADC module */
	fsp_err = R_ADC_B_Open(p_dcb->hadc, p_dcb->cadc);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	R_ADC_B_CallbackSet(p_dcb->hadc, HAL_ADHB_Callback, p_dcb, NULL);

	 /* Configures the ADC scan parameters */
	fsp_err = R_ADC_B_ScanCfg(p_dcb->hadc, p_dcb->sadc);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	/* Calibrate the ADC */
	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	fsp_err = R_ADC_B_Calibrate( p_dcb->hadc, NULL);
	if(fsp_err != FSP_SUCCESS && fsp_err != FSP_ERR_UNSUPPORTED) return E_IO;

	if( fsp_err == FSP_SUCCESS) {
		err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_ADC_CALIB_TMOUT);
	}

	return err;
}

/*
 * Close Device
 */
LOCAL ER dev_adhb_closefn( ID devid, UINT option, T_MSDI *p_msdi)
{
	T_HAL_ADHB_DCB	*p_dcb;
	fsp_err_t	fsp_err;

	p_dcb = (T_HAL_ADHB_DCB*)(p_msdi->dmsdi.exinf);
	if(p_dcb->hadc == NULL) return E_IO;
	
	/* Stop ADC scan */
	R_ADC_B_ScanStop(p_dcb->hadc);

	/* Close the ADC module */
	fsp_err = R_ADC_B_Close(p_dcb->hadc);

	return (fsp_err == FSP_SUCCESS)? E_OK: E_IO;
}

/*
 * Read Device
 */
LOCAL ER dev_adhb_readfn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_HAL_ADHB_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_HAL_ADHB_DCB*)(p_msdi->dmsdi.exinf);
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
LOCAL ER dev_adhb_writefn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_HAL_ADHB_DCB	*p_dcb;
	ER		rtn;

	p_dcb = (T_HAL_ADHB_DCB*)(p_msdi->dmsdi.exinf);
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
LOCAL ER dev_adhb_eventfn( INT evttyp, void *evtinf, T_MSDI *p_msdi)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */
EXPORT ER dev_init_hal_adhb( UW unit, adc_b_instance_ctrl_t *hadc,
				const adc_cfg_t *cadc, const adc_b_scan_cfg_t *sadc)
{
	T_HAL_ADHB_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_HAL_ADHB_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_HAL_ADHB_DCB*)Kmalloc(sizeof(T_HAL_ADHB_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_adhb_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_adhb_cb[unit];
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
	dmsdi.openfn	= dev_adhb_openfn;
	dmsdi.closefn	= dev_adhb_closefn;
	dmsdi.readfn	= dev_adhb_readfn;
	dmsdi.writefn	= dev_adhb_writefn;
	dmsdi.eventfn	= dev_adhb_eventfn;
	
	knl_strcpy( (char*)dmsdi.devnm, DEVNAME_HAL_ADHB);
	i = knl_strlen(DEVNAME_HAL_ADHB);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_1;

	p_dcb->hadc	= hadc;
	p_dcb->cadc	= cadc;
	p_dcb->sadc	= sadc;
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

#endif		/* DEVCNF_USE_HAL_ADHB */
#endif		/* MTKBSP_RAFSP */
