/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2021-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	adc_hal_xmc7200.h
 *	A/D converter device driver
 *	System-dependent definition for XMC7200
 */

#ifndef	__DEV_ADC_HAL_XMC7200_H__
#define	__DEV_ADC_HAL_XMC7200_H__

void adc_hal_init( void );
void adc_hal_init_clock( void );
int adc_hal_open( void );
void adc_hal_close();
void adc_hal_start_convert( void );
unsigned int adc_hal_get_result( unsigned int ch);
void adc_hal_clear_interrupt( void );

#endif		/* __DEV_ADC_HAL_XMC7200_H__ */
