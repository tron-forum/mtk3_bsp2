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

/*
 *	adc_sysdep.h
 *	A/D converter device driver
 *	System-dependent definition for XMC7200
 */

#ifndef	_DEV_HAL_ADC_XMC7200_H_
#define	_DEV_HAL_ADC_XMC7200_H_

#define	DEV_ADC_1	(0)	/* SAR ADC1 */

/* NUmber of A/DC chanels */
#define	ADC_CH_NUM	(17)
#define	ADC_MAX_SQ	(17)

/*
 * A/D converter interrupt number
*/
#define	INTNO_INTSAR1_CH16	171

#endif		/* _DEV_HAL_ADC_XMC7200_H_ */
