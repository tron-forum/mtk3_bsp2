/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <config_bsp/ra_fsp/config_bsp.h>

#ifdef MTKBSP_RAFSP
#if DEVCNF_USE_HAL_I3C_IIC

#include <stdlib.h>

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/ra_fsp/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "hal_i3c_i2c_cnf.h"

/*
 *	hal_i3c_i2c.c
 *	I3C-I2C device driver (RA FPS)
*/

/*---------------------------------------------------------------------*/
/*I3C Device driver Control block
 */
typedef struct {
	i3c_instance_ctrl_t		*hi3c;		// I3C handle
	i3c_cfg_t			*ci3c;		// I3C config
	ID				devid;		// Device ID
	UINT				omode;		// Open mode
	UW				unit;		// Unit no
	ER				err;		// Error code that occurred during interrupt processing
	ID				evtmbfid;	// MBF ID for event notification
	UW				dmode;		// Device mode
	UW				tadr;		// Target Address
} T_HAL_I3C_I2C_DCB;

/* Interrupt detection flag */
LOCAL ID	id_flgid;
LOCAL T_CFLG	id_flg	= {
		.flgatr		= TA_TFIFO | TA_WMUL,
		.iflgptn	= 0,
};

#if TK_SUPPORT_MEMLIB
LOCAL T_HAL_I3C_I2C_DCB	*dev_i3c_cb[DEV_HAL_I3C_I2C_UNITNM];
#define		get_dcb_ptr(unit)	(dev_i3c_cb[unit])
#else
LOCAL T_HAL_I3C_I2C_DCB	dev_i3c_cb[DEV_HAL_I3C_I2C_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_I3C_cb[unit])
#endif

/*---------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_HAL_I3C_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	if((req->start >= TDN_HAL_I3C_I2C_MODE) && (req->start <= TDN_HAL_I3C_I2C_MAX)) return E_PAR;

	req->asize = sizeof(UW);
	
	if(req->size == 0) return E_OK;
	else if(req->size != sizeof(UW)) return E_PAR;

	switch(req->start) {
	case TDN_HAL_I3C_I2C_MODE:
		*(UW*)req->buf = p_dcb->dmode;
		break;
	case TDN_HAL_I3C_I2C_TADR:
		*(UW*)req->buf = p_dcb->tadr;
		break;
	default:
		return E_PAR;
	}
	return E_OK;
}

LOCAL ER write_atr(T_HAL_I3C_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	UW	data;

	if(req->size != sizeof(UW)) return E_PAR;
	data = *(UW*)req->buf;

	switch(req->start) {
	case TDN_HAL_I3C_I2C_MODE:
		if(data <HAL_I3C_I2C_MODE_CNT || data > HAL_I3C_I2C_MODE_TAR) return E_PAR;
		p_dcb->dmode = data;
		break;
	case TDN_HAL_I3C_I2C_TADR:
		p_dcb->tadr = data;
		break;
	default:
		return E_PAR;
	}
	req->asize = sizeof(UW);
	return E_OK;
}

/*---------------------------------------------------------------------*/
/*Device-specific data control
 */

/* HAL Callback functions */
void g_i3c0_callback(i3c_callback_args_t const *const p_args)
{
	T_HAL_I3C_I2C_DCB	*p_dcb;

	ENTER_TASK_INDEPENDENT

	p_dcb = (T_HAL_I3C_I2C_DCB*)p_args->p_context;
	tk_set_flg(id_flgid, 1<< p_dcb->unit);

	switch(p_args->event) {
		case I3C_EVENT_WRITE_COMPLETE:
		case I3C_EVENT_READ_COMPLETE:
			p_dcb->err = E_OK;
			break;
		default:
			p_dcb->err = E_IO;
			break;
	}
	LEAVE_TASK_INDEPENDENT
}

LOCAL i3c_device_cfg_t device_cfg = {
	.static_address		= 0,
	.dynamic_address	= 0
};

LOCAL i3c_device_table_cfg_t device_table_cfg = {
	.static_address        = 0,
	.dynamic_address       = 0,
	.device_protocol       = I3C_DEVICE_PROTOCOL_I2C,
	.ibi_accept            = false, 
	.ibi_payload           = false,
	.master_request_accept = false,
};

LOCAL ER read_data(T_HAL_I3C_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	UINT		wflgptn, rflgptn;
	fsp_err_t	fsp_err;
	ER		err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	switch(p_dcb->dmode) {
	case HAL_I3C_I2C_MODE_CNT:
		fsp_err = R_I3C_DeviceCfgSet(p_dcb->hi3c, &device_cfg);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		device_table_cfg.static_address = req->start;
		fsp_err = R_I3C_MasterDeviceTableSet(p_dcb->hi3c, 0, &device_table_cfg);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		fsp_err = R_I3C_Enable(p_dcb->hi3c);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		fsp_err = R_I3C_DeviceSelect(p_dcb->hi3c, 0, I3C_BITRATE_MODE_I2C_STDBR);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		fsp_err = R_I3C_Read(p_dcb->hi3c, (UB*)req->buf, req->size, false);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		break;

	case HAL_I3C_I2C_MODE_TAR:
		return E_NOSPT;
	default:
		return E_SYS;
	}

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I3C_I2C_TMOUT);
	if(err >= E_OK) {
		err = p_dcb->err;
		if(err >= E_OK) req->asize = req->size;
	}

	return err;
}

LOCAL ER write_data(T_HAL_I3C_I2C_DCB *p_dcb, T_DEVREQ *req)
{	
	UINT		wflgptn, rflgptn;
	fsp_err_t	fsp_err;
	ER		err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	switch(p_dcb->dmode) {
	case HAL_I3C_I2C_MODE_CNT:
		fsp_err = R_I3C_DeviceCfgSet(p_dcb->hi3c, &device_cfg);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		device_table_cfg.static_address = req->start;
		fsp_err = R_I3C_MasterDeviceTableSet(p_dcb->hi3c, 0, &device_table_cfg);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		fsp_err = R_I3C_Enable(p_dcb->hi3c);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		fsp_err = R_I3C_DeviceSelect(p_dcb->hi3c, 0, I3C_BITRATE_MODE_I2C_STDBR);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		fsp_err = R_I3C_Write(p_dcb->hi3c, (UB*)req->buf, req->size, false);
		if(fsp_err != FSP_SUCCESS) return E_IO;

		break;

	case HAL_I3C_I2C_MODE_TAR:
		return E_NOSPT;
	default:
		return E_SYS;
	}

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I3C_I2C_TMOUT);
	if(err >= E_OK) {
		err = p_dcb->err;
		if(err >= E_OK) req->asize = req->size;
	}

	return err;
}

/*----------------------------------------------------------------------
 * mSDI I/F function
 */
/*
 * Open device
 */
LOCAL ER dev_i2c_openfn( ID devid, UINT omode, T_MSDI *msdi)
{
	T_HAL_I3C_I2C_DCB	*p_dcb;
	fsp_err_t	fsp_err;

	p_dcb = (T_HAL_I3C_I2C_DCB*)(msdi->dmsdi.exinf);
	if(p_dcb->hi3c == NULL) return E_IO;

	p_dcb->omode = omode;

	p_dcb->ci3c->p_context = p_dcb;
	fsp_err =  R_I3C_Open(p_dcb->hi3c, p_dcb->ci3c);

	return (fsp_err == FSP_SUCCESS)?E_OK:E_IO;
}

/*
 * Close Device
 */
LOCAL ER dev_i2c_closefn( ID devid, UINT option, T_MSDI *msdi)
{
	return E_OK;
}

/*
 * Read Device
 */
LOCAL ER dev_i2c_readfn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_HAL_I3C_I2C_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_HAL_I3C_I2C_DCB*)(p_msdi->dmsdi.exinf);

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
LOCAL ER dev_i2c_writefn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_HAL_I3C_I2C_DCB	*p_dcb;
	ER		rtn;

	p_dcb = (T_HAL_I3C_I2C_DCB*)(p_msdi->dmsdi.exinf);

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
LOCAL ER dev_i2c_eventfn( INT evttyp, void *evtinf, T_MSDI *msdi)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */
EXPORT ER dev_init_hal_i3c_i2c( UW unit, i3c_instance_ctrl_t *hi3c, const i3c_cfg_t *ci3c)
{
	static i3c_cfg_t	ci3c_nc;

	T_HAL_I3C_I2C_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_HAL_I3C_I2C_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_HAL_I3C_I2C_DCB*)Kmalloc(sizeof(T_HAL_I3C_I2C_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_i3c_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_i3c_cb[unit];
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
	dmsdi.openfn	= dev_i2c_openfn;
	dmsdi.closefn	= dev_i2c_closefn;
	dmsdi.readfn	= dev_i2c_readfn;
	dmsdi.writefn	= dev_i2c_writefn;
	dmsdi.eventfn	= dev_i2c_eventfn;
	
	knl_strcpy( (char*)dmsdi.devnm, DEVNAME_HAL_I3C_I2C);
	i = knl_strlen(DEVNAME_HAL_I3C_I2C);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_1;

	ci3c_nc = *ci3c;

	p_dcb->hi3c	= hi3c;
	p_dcb->ci3c	= &ci3c_nc;
	p_dcb->devid	= p_msdi->devid;
	p_dcb->unit	= unit;
	p_dcb->evtmbfid	= idev.evtmbfid;
	p_dcb->dmode	= HAL_I3C_I2C_MODE_CNT;

	return E_OK;

err_1:
#if TK_SUPPORT_MEMLIB
	Kfree(p_dcb);
#endif
	return err;
}

/*----------------------------------------------------------------------
 * I3C register access support function
 */

EXPORT ER hal_i3c_i2c_read_reg(ID dd, UW sadr, UW radr, UB *data)
{
	fsp_err_t		fsp_err;
	T_HAL_I3C_I2C_DCB	*p_dcb;
	ID			devid;
	UINT			unit;
	UINT			wflgptn, rflgptn;
	UB			adr8b;
	ER			err;
	
	devid = tk_oref_dev(dd, NULL);
	for(unit = 0; unit <DEV_HAL_I3C_I2C_UNITNM; unit++) {
		p_dcb = get_dcb_ptr(unit);
		if(p_dcb->devid == devid) break;
	}
	if(unit >= DEV_HAL_I3C_I2C_UNITNM) return E_ID;

	wflgptn = 1 << unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	fsp_err = R_I3C_DeviceCfgSet(p_dcb->hi3c, &device_cfg);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	device_table_cfg.static_address = sadr;
	fsp_err = R_I3C_MasterDeviceTableSet(p_dcb->hi3c, 0, &device_table_cfg);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	fsp_err = R_I3C_Enable(p_dcb->hi3c);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	fsp_err = R_I3C_DeviceSelect(p_dcb->hi3c, 0, I3C_BITRATE_MODE_I2C_STDBR);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	adr8b = (UB)radr;
	fsp_err = R_I3C_Write(p_dcb->hi3c,  &adr8b, 1, true);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I3C_I2C_TMOUT);
	if(err < E_OK) return E_IO;

	fsp_err = R_I3C_Read(p_dcb->hi3c, data, 1, false);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I3C_I2C_TMOUT);
	
	return err;
}

EXPORT ER hal_i3c_i2c_write_reg(ID dd, UW sadr, UW radr, UB data)
{
	fsp_err_t		fsp_err;
	T_HAL_I3C_I2C_DCB	*p_dcb;
	ID			devid;
	UINT			unit;
	UINT			wflgptn, rflgptn;
	UB			sdat[2];
	ER			err;
	
	devid = tk_oref_dev(dd, NULL);
	for(unit = 0; unit <DEV_HAL_I3C_I2C_UNITNM; unit++) {
		p_dcb = get_dcb_ptr(unit);
		if(p_dcb->devid == devid) break;
	}
	if(unit >= DEV_HAL_I3C_I2C_UNITNM) return E_ID;

	wflgptn = 1 << unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	fsp_err = R_I3C_DeviceCfgSet(p_dcb->hi3c, &device_cfg);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	device_table_cfg.static_address = sadr;
	fsp_err = R_I3C_MasterDeviceTableSet(p_dcb->hi3c, 0, &device_table_cfg);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	fsp_err = R_I3C_Enable(p_dcb->hi3c);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	fsp_err = R_I3C_DeviceSelect(p_dcb->hi3c, 0, I3C_BITRATE_MODE_I2C_STDBR);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	sdat[0] = (UB)radr; sdat[1] = data;
	fsp_err = R_I3C_Write(p_dcb->hi3c, sdat, sizeof(sdat), false);
	if(fsp_err != FSP_SUCCESS) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I3C_I2C_TMOUT);
	
	return err;
}

#endif		/* DEVCNF_USE_HAL_I3C_IIC */
#endif		/* MTKBSP_RAFSP */
