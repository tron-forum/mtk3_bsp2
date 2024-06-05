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

#ifndef	_DEV_HAL_ADC_H_
#define	_DEV_HAL_ADC_H_
/*
 *	hal_adc.h
 *	A/D converter device driver 
 *	Driver local definition
*/
#define DEV_HAL_ADC0	0
#define DEV_HAL_ADC1	1
#define DEV_HAL_ADC2	2

/*----------------------------------------------------------------------*/
/* Device driver Control block
 */
typedef struct {
	UW	unit;		/* Unit No. */
	UINT	omode;		/* Open mode */

	/* Attribute data */
	ID	evtmbfid;	/* MBF ID for event notification */
} T_ADC_DCB;

/*----------------------------------------------------------------------*/
/* Device driver low-level interface
 */

/* The following functions are defined in the system-dependent part. */
IMPORT W dev_adc_llctl( UW, INT, UW, UW, UW*);	/* Low-Level device control */
IMPORT ER dev_adc_llinit( T_ADC_DCB*);		/* Low-level device initialization */

/* Low-level device control command */
typedef enum {
	LLD_ADC_OPEN,
	LLD_ADC_CLOSE,
	LLD_ADC_READ,
	LLD_ADC_RSIZE,
} T_LLD_ADC_CMD;

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */

IMPORT ER dev_init_hal_adc( UW unit );

#endif		/* _DEV_HAL_ADC_H_ */
