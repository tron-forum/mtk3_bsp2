/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <config_bsp/nxp_mcux/config_bsp.h>

#ifdef MTKBSP_MPCXPRESSO
#if DEVCNF_USE_HAL_LPI2C

#include "peripherals.h"

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/nxp_mcux/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "hal_lpi2c_cnf.h"

/*
 *	hal_lpi2c.c
 *	I2C device driver (NXP MCUXPresso)
*/

/*---------------------------------------------------------------------*/
/*I2C Device driver Control block
 */
typedef struct {
	lpi2c_master_handle_t		hi2c;		// LPI2C handle
	LPI2C_Type			*base;		// LPI2C peripheral base address

	ID				devid;		// Device ID
	UINT				omode;		// Open mode
	UW				unit;		// Unit no
	ER				err;		// Error code that occurred during interrupt processing
	ID				evtmbfid;	// MBF ID for event notification
	UW				dmode;		// Device mode
	UW				tadr;		// Target Address
} T_HAL_LPI2C_DCB;

/* Interrupt detection flag */
LOCAL ID	id_flgid;
LOCAL T_CFLG	id_flg	= {
			.flgatr		= TA_TFIFO | TA_WMUL,
			.iflgptn	= 0,
};

#if TK_SUPPORT_MEMLIB
LOCAL T_HAL_LPI2C_DCB	*dev_lpi2c_cb[DEV_HAL_LPI2C_UNITNM];
#define		get_dcb_ptr(unit)	(dev_lpi2c_cb[unit])
#else
LOCAL T_HAL_LPI2C_DCB	dev_lpi2c_cb[DEV_HAL_LPI2C_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_I2C_cb[unit])
#endif

/*---------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_HAL_LPI2C_DCB *p_dcb, T_DEVREQ *req)
{
	if((req->start >= TDN_HAL_I2C_MODE) && (req->start <= TDN_HAL_I2C_MAX)) return E_PAR;

	req->asize = sizeof(UW);
	
	if(req->size == 0) return E_OK;
	else if(req->size != sizeof(UW)) return E_PAR;

	switch(req->start) {
	case TDN_HAL_I2C_MODE:
		*(UW*)req->buf = p_dcb->dmode;
		break;
	case TDN_HAL_I2C_TADR:
		*(UW*)req->buf = p_dcb->tadr;
		break;
	default:
		return E_PAR;
	}
	return E_OK;
}

LOCAL ER write_atr(T_HAL_LPI2C_DCB *p_dcb, T_DEVREQ *req)
{
	UW	data;

	if(req->size != sizeof(UW)) return E_PAR;
	data = *(UW*)req->buf;

	switch(req->start) {
	case TDN_HAL_I2C_MODE:
		if(data <HAL_I2C_MODE_CNT || data > HAL_I2C_MODE_TAR) return E_PAR;
		p_dcb->dmode = data;
		break;
	case TDN_HAL_I2C_TADR:
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
LOCAL void lpi2c_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle,
			status_t status, void *userData)
{
	T_HAL_LPI2C_DCB		*p_dcb;

	ENTER_TASK_INDEPENDENT

	p_dcb = (T_HAL_LPI2C_DCB*)userData;
	p_dcb->err = (status == kStatus_Success)? E_OK: E_IO;

	tk_set_flg(id_flgid, 1<<(p_dcb->unit));

	LEAVE_TASK_INDEPENDENT
}


LOCAL ER read_data(T_HAL_LPI2C_DCB *p_dcb, T_DEVREQ *req)
{
	lpi2c_master_transfer_t masterXfer = {0};
	status_t reVal;

	UINT		wflgptn, rflgptn;
	ER		err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	switch(p_dcb->dmode) {
	case HAL_I2C_MODE_CNT:
		masterXfer.slaveAddress   = req->start;
		masterXfer.direction      = kLPI2C_Read;
		masterXfer.subaddress     = 0;
		masterXfer.subaddressSize = 0;
		masterXfer.data           = req->buf;
		masterXfer.dataSize       = req->size;
		masterXfer.flags          = kLPI2C_TransferDefaultFlag;
		reVal = LPI2C_MasterTransferNonBlocking(p_dcb->base, &(p_dcb->hi2c), &masterXfer);
		if (reVal != kStatus_Success) {
			return E_IO;
		}

		break;

	case HAL_I2C_MODE_TAR:
		return E_NOSPT;
	default:
		return E_SYS;
	}

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_LPI2C_TMOUT);
	if(err >= E_OK) {
		err = p_dcb->err;
		if(err >= E_OK) req->asize = req->size;
	}

	return err;
}

LOCAL ER write_data(T_HAL_LPI2C_DCB *p_dcb, T_DEVREQ *req)
{
	lpi2c_master_transfer_t masterXfer = {0};
	status_t reVal;
	
	UINT		wflgptn, rflgptn;
	ER		err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	switch(p_dcb->dmode) {
	case HAL_I2C_MODE_CNT:
		masterXfer.slaveAddress   = req->start;
		masterXfer.direction      = kLPI2C_Write;
		masterXfer.subaddress     = 0;
		masterXfer.subaddressSize = 0;
		masterXfer.data           = req->buf;
		masterXfer.dataSize       = req->size;
		masterXfer.flags          = kLPI2C_TransferDefaultFlag;
		reVal = LPI2C_MasterTransferNonBlocking(p_dcb->base, &(p_dcb->hi2c), &masterXfer);
		if (reVal != kStatus_Success) {
			return E_IO;
		}
		break;

	case HAL_I2C_MODE_TAR:
		return E_NOSPT;
	default:
		return E_SYS;
	}

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_LPI2C_TMOUT);
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
	T_HAL_LPI2C_DCB		*p_dcb;

	p_dcb = (T_HAL_LPI2C_DCB*)(msdi->dmsdi.exinf);
	if(p_dcb->base == NULL) return E_IO;

	p_dcb->omode = omode;

	LPI2C_MasterTransferCreateHandle(p_dcb->base, &(p_dcb->hi2c), lpi2c_callback, p_dcb);

	return E_OK;
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
	T_HAL_LPI2C_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_HAL_LPI2C_DCB*)(p_msdi->dmsdi.exinf);

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
	T_HAL_LPI2C_DCB	*p_dcb;
	ER		rtn;

	p_dcb = (T_HAL_LPI2C_DCB*)(p_msdi->dmsdi.exinf);

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
EXPORT ER dev_init_hal_lpi2c( UW unit, LPI2C_Type *base)
{
	T_HAL_LPI2C_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_HAL_LPI2C_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_HAL_LPI2C_DCB*)Kmalloc(sizeof(T_HAL_LPI2C_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_lpi2c_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_lpi2c_cb[unit];
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
	
	knl_strcpy( (char*)dmsdi.devnm, DEVNAME_HAL_LPI2C);
	i = knl_strlen(DEVNAME_HAL_LPI2C);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_1;

	p_dcb->base	= base;
	p_dcb->devid	= p_msdi->devid;
	p_dcb->unit	= unit;
	p_dcb->evtmbfid	= idev.evtmbfid;
	p_dcb->dmode	= HAL_I2C_MODE_CNT;

	return E_OK;

err_1:
#if TK_SUPPORT_MEMLIB
	Kfree(p_dcb);
#endif
	return err;
}

/*----------------------------------------------------------------------
 * I2C register access support function
 */

EXPORT ER hal_lpi2c_read_reg(ID dd, UW sadr, UW radr, UB *data)
{
	T_HAL_LPI2C_DCB		*p_dcb;
	ID			devid;
	UINT			unit;
	UINT			wflgptn, rflgptn;
	ER			err;

	lpi2c_master_transfer_t	masterXfer = {0};
	status_t		reVal;

	devid = tk_oref_dev(dd, NULL);
	for(unit = 0; unit <DEV_HAL_LPI2C_UNITNM; unit++) {
		p_dcb = get_dcb_ptr(unit);
		if(p_dcb->devid == devid) break;
	}
	if(unit >= DEV_HAL_LPI2C_UNITNM) return E_ID;

	masterXfer.slaveAddress   = sadr;
	masterXfer.direction      = kLPI2C_Read;
	masterXfer.subaddress     = radr;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = data;
	masterXfer.dataSize       = 1;
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;


	wflgptn = 1 << unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	reVal = LPI2C_MasterTransferNonBlocking(p_dcb->base, &(p_dcb->hi2c), &masterXfer);
	if (reVal != kStatus_Success) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_LPI2C_TMOUT);
	
	return err;
}

EXPORT ER hal_lpi2c_write_reg(ID dd, UW sadr, UW radr, UB data)
{
	T_HAL_LPI2C_DCB		*p_dcb;
	ID			devid;
	UINT			unit;
	UINT			wflgptn, rflgptn;
	ER			err;

	lpi2c_master_transfer_t	masterXfer = {0};
	status_t		reVal;
	
	devid = tk_oref_dev(dd, NULL);
	for(unit = 0; unit <DEV_HAL_LPI2C_UNITNM; unit++) {
		p_dcb = get_dcb_ptr(unit);
		if(p_dcb->devid == devid) break;
	}
	if(unit >= DEV_HAL_LPI2C_UNITNM) return E_ID;

	wflgptn = 1 << unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	masterXfer.slaveAddress   = sadr;
	masterXfer.direction      = kLPI2C_Write;
	masterXfer.subaddress     = radr;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = &data;
	masterXfer.dataSize       = 1;
	masterXfer.flags          = kLPI2C_TransferDefaultFlag;

	reVal = LPI2C_MasterTransferNonBlocking(p_dcb->base, &(p_dcb->hi2c), &masterXfer);
	if (reVal != kStatus_Success) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_LPI2C_TMOUT);
	
	return err;
}

#endif		/* DEVCNF_USE_HAL_LPI2C */
#endif		/* MTKBSP_MPCXPRESSO */