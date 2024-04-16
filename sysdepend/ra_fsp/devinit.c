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
#ifdef MTKBSP_RAFSP

/*
 *	devinit.c (RA FSP)
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

#if DEVCNF_USE_HAL_IIC
	err = dev_init_hal_i2c(0, &g_i2c_master0_ctrl, &g_i2c_master0_cfg);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_SCI_IIC
	err = dev_init_hal_sci_i2c(0, &g_i2c0_ctrl, &g_i2c0_cfg);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_I3C_IIC
	err = dev_init_hal_i3c_i2c(0, &g_i3c0_ctrl, &g_i3c0_cfg);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_ADC
	err = dev_init_hal_adc( 0, &g_adc0_ctrl, &g_adc0_cfg, &g_adc0_channel_cfg);
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
#endif /* MTKBSP_RAFSP */
