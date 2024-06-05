/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_cnf_sysdep.h 
 *	A/D converter device driver configuration file
 *		for XMC7200
 */
#ifndef	__DEV_ADC_CNF_XMC7200_H__
#define	__DEV_ADC_CNF_XMC7200_H__

/* Device clock initialization */
#define DEVCONF_ADC_INIT_MCLK	TRUE	// Initialize module clock

/* ADC1 */
/* 
 * Sampling time 
 * In A/DC clock cycles. The minimum time needed for the proper settling is at least 300ns, 
 * i.e. 6 clock cycles at the max frequency of 20MHz.
 */
#define	DEVCONF_SAR1_SMPTIME	120U

/* Interrupt t priority */
#define	DEVCNF_SAR1_INTPRI	5

#endif		/* __DEV_ADC_CNF_XMC7200_H__ */
