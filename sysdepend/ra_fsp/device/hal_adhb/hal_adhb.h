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

#ifndef	_DEV_HAL_ADHB_H_
#define	_DEV_HAL_ADHB_H_
/*
 *	hal_adhb.h
 *	A/DC(ADC16H) device driver (RA FSP)
*/

/*----------------------------------------------------------------------
 * A/DC Device
 */
#define DEV_HAL_ADHB1	0

/*----------------------------------------------------------------------
 * Attribute data
 */
#define TDN_HAL_ADHB_MODE	(-100)	// A/DC Mode

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */

IMPORT ER dev_init_hal_adhb( UW unit, adc_b_instance_ctrl_t *hadc,
				const adc_cfg_t *cadc, const adc_b_scan_cfg_t *sadc);

#endif	/* _DEV_HAL_ADHB_H_ */