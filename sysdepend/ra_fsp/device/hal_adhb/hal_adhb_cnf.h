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

/*
 *	hal_adhb_cnf.h 
 *	A/DC(ADC16H) device driver  (RA FSP)
 *		Device configuration file
 */
#ifndef	_DEV_HAL_ADHB_CNF_H_
#define	_DEV_HAL_ADHB_CNF_H_

#define DEVNAME_HAL_ADHB		"hadhb"
#define DEV_HAL_ADHB_TMOUT		(500)
#define DEV_HAL_ADHB_CALIB_TMOUT	(1000)

#define DEV_HAL_ADHB_UNITNM		(3)	// Number of A/DC units (max 26)

#endif	/* _DEV_HAL_ADHB_CNF_H_ */