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
#if DEVCNF_USE_HAL_IIC

#include <stdlib.h>

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/stm32_cube/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "hal_i2c_cnf.h"

/*
 *	hal_i2c.c
 *	I2C device driver (STM32Cube FW)
*/

/*---------------------------------------------------------------------*/
/*I2C Device driver Control block
 */
typedef struct {
	I2C_HandleTypeDef	*hi2c;		// HAL handle
	ID			devid;		// Device ID
	UINT			omode;		// Open mode
	UW			unit;		// Unit no
	ER			err;		// Error code that occurred during interrupt processing
	ID			evtmbfid;	// MBF ID for event notification
	UW			dmode;		// Device mode
	UW			tadr;		// Target Address
} T_HAL_I2C_DCB;

/* Interrupt detection flag */
LOCAL ID	id_flgid;
LOCAL T_CFLG	id_flg	= {
			.flgatr		= TA_TFIFO | TA_WMUL,
			.iflgptn	= 0,
};

#if TK_SUPPORT_MEMLIB
LOCAL T_HAL_I2C_DCB	*dev_i2c_cb[DEV_HAL_I2C_UNITNM];
#define		get_dcb_ptr(unit)	(dev_i2c_cb[unit])
#else
LOCAL T_HAL_I2C_DCB	dev_i2c_cb[DEV_HAL_I2C_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_I2C_cb[unit])
#endif

/*---------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_HAL_I2C_DCB *p_dcb, T_DEVREQ *req)
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

LOCAL ER write_atr(T_HAL_I2C_DCB *p_dcb, T_DEVREQ *req)
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
LOCAL void HAL_I2C_Callback(I2C_HandleTypeDef *hi2c, ER err)
{
	T_HAL_I2C_DCB	*p_dcb;
	UINT	i;

	ENTER_TASK_INDEPENDENT

	for(i = 0; i < DEV_HAL_I2C_UNITNM; i++) {
		p_dcb = get_dcb_ptr(i);
		if(p_dcb->hi2c == hi2c) {
			p_dcb->err = err;
			tk_set_flg(id_flgid, 1<< p_dcb->unit);
			break;
		}
	}

	LEAVE_TASK_INDEPENDENT
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_OK);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_OK);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_OK);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_OK);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_OK);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_OK);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_IO);
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Callback(hi2c, E_ABORT);
}

LOCAL ER read_data(T_HAL_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	HAL_StatusTypeDef	hal_sts;
	UINT			wflgptn, rflgptn;
	ER			err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	switch(p_dcb->dmode) {
	case HAL_I2C_MODE_CNT:
		hal_sts = HAL_I2C_Master_Receive_IT(
			p_dcb->hi2c,		// I2C_Handle
			(req->start)<<1,	// Target device address
			req->buf,		// Pointer to data buffer
			req->size		// Amount of data to be sent
		);
		break;
	case HAL_I2C_MODE_TAR:
		hal_sts = HAL_I2C_Slave_Receive_IT(
			p_dcb->hi2c,		// I2C_Handle
			req->buf,		// Pointer to data buffer
			req->size		// Amount of data to be sent
		);
		break;
	default:
		return E_SYS;
	}
	if(hal_sts != HAL_OK) return E_BUSY;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I2C_TMOUT);
	if(err >= E_OK) {
		err  = p_dcb->err;
		if(err >= E_OK) req->asize = req->size;
	}

	return err;
}

LOCAL ER write_data(T_HAL_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	HAL_StatusTypeDef	hal_sts;
	UINT			wflgptn, rflgptn;
	ER			err;

	wflgptn = 1 << p_dcb->unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	switch(p_dcb->dmode) {
	case HAL_I2C_MODE_CNT:
		hal_sts = HAL_I2C_Master_Transmit_IT(
			p_dcb->hi2c,		// I2C_Handle
			(req->start)<<1,	// Target device address
			req->buf,		// Pointer to data buffer
			req->size		// Amount of data to be sent
		);
		break;
	case HAL_I2C_MODE_TAR:
		hal_sts = HAL_I2C_Slave_Transmit_IT(
			p_dcb->hi2c,		// I2C_Handle
			req->buf,		// Pointer to data buffer
			req->size		// Amount of data to be sent
		);
		break;
	default:
		return E_SYS;
	}
	if(hal_sts != HAL_OK) return E_BUSY;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I2C_TMOUT);
	if(err >= E_OK) {
		err  = p_dcb->err;
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
	T_HAL_I2C_DCB	*p_dcb;

	p_dcb = (T_HAL_I2C_DCB*)(msdi->dmsdi.exinf);
	p_dcb->omode = omode;
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
	T_HAL_I2C_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_HAL_I2C_DCB*)(p_msdi->dmsdi.exinf);

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
	T_HAL_I2C_DCB	*p_dcb;
	ER		rtn;

	p_dcb = (T_HAL_I2C_DCB*)(p_msdi->dmsdi.exinf);

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
EXPORT ER dev_init_hal_i2c( UW unit, I2C_HandleTypeDef *hi2c )
{
	T_HAL_I2C_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_HAL_I2C_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_HAL_I2C_DCB*)Kmalloc(sizeof(T_HAL_I2C_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_i2c_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_i2c_cb[unit];
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
	
	knl_strcpy( (char*)dmsdi.devnm, DEVNAME_HAL_I2C);
	i = knl_strlen(DEVNAME_HAL_I2C);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_1;

	p_dcb->hi2c	= hi2c;
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

EXPORT ER i2c_read_reg(ID dd, UW sadr, UW radr, UB *data)
{
	HAL_StatusTypeDef	hal_sts;
	T_HAL_I2C_DCB		*p_dcb;
	ID			devid;
	UINT			unit;
	UINT			wflgptn, rflgptn;
	ER			err;

	devid = tk_oref_dev(dd, NULL);
	for(unit = 0; unit <DEV_HAL_I2C_UNITNM; unit++) {
		p_dcb = get_dcb_ptr(unit);
		if(p_dcb->devid == devid) break;
	}
	if(unit >= DEV_HAL_I2C_UNITNM) return E_ID;

	wflgptn = 1 << unit;
	tk_clr_flg(id_flgid, ~wflgptn);
	
	hal_sts = HAL_I2C_Mem_Read_IT(p_dcb->hi2c, (uint16_t)(sadr<<1),
					(uint16_t)radr, I2C_MEMADD_SIZE_8BIT, data, 1);
	if(hal_sts != HAL_OK) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I2C_TMOUT);
	if(err >= E_OK) err  = p_dcb->err;
	
	return err;
}

EXPORT ER i2c_write_reg(ID dd, UW sadr, UW radr, UB data)
{
	HAL_StatusTypeDef	hal_sts;
	T_HAL_I2C_DCB		*p_dcb;
	ID			devid;
	UINT			unit;
	UINT			wflgptn, rflgptn;
	ER			err;
	
	devid = tk_oref_dev(dd, NULL);
	for(unit = 0; unit <DEV_HAL_I2C_UNITNM; unit++) {
		p_dcb = get_dcb_ptr(unit);
		if(p_dcb->devid == devid) break;
	}
	if(unit >= DEV_HAL_I2C_UNITNM) return E_ID;

	wflgptn = 1 << unit;
	tk_clr_flg(id_flgid, ~wflgptn);

	hal_sts = HAL_I2C_Mem_Write_IT(p_dcb->hi2c, (uint16_t)(sadr<<1),
					(uint16_t)radr, I2C_MEMADD_SIZE_8BIT, &data, 1);
	if(hal_sts != HAL_OK) return E_IO;

	err = tk_wai_flg(id_flgid, wflgptn, TWF_ANDW | TWF_BITCLR, &rflgptn, DEV_HAL_I2C_TMOUT);
	if(err >= E_OK) err  = p_dcb->err;
	
	return err;
}

#endif		/* DEVCNF_USE_HAL_IIC */
#endif		/* MTKBSP_STM32CUBE */