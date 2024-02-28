/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef MTKBSP_MPCXPRESSO

/*
 *	devinit.c (LCP MPCXpresso)
 *	Device-Dependent Initialization
 */
#include <tk/tkernel.h>
#include <tk/device.h>
#include <kernel.h>

/* ------------------------------------------------------------------------ */

/*
 * Initialization before micro T-Kernel starts
 */

EXPORT ER knl_init_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Start processing after T-Kernel starts
 *	Called from the initial task contexts.
 */
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

#if DEVCNF_USE_HAL_LPI2C
	err = dev_init_hal_lpi2c( 0, LPI2C2);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_ADC
	err = dev_init_hal_adc( 0, ADC0);
#endif

	return err;
}

#if USE_SHUTDOWN
/* ------------------------------------------------------------------------ */
/*
 * System finalization
 *	Called just before system shutdown.
 *	Execute finalization that must be done before system shutdown.
 */
EXPORT ER knl_finish_device( void )
{
	return E_OK;
}

#endif /* USE_SHUTDOWN */
#endif /* MTKBSP_MPCXPRESSO */
