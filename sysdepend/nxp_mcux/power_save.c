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

#include <tk/tkernel.h>
#include <kernel.h>

/*
 *	power_save.c (LCP MPCXpresso)
 *	Power-Saving Function
 */

/*
 * Switch to power-saving mode
 */
EXPORT void low_pow( void )
{
}

/*
 * Move to suspend mode
 */
EXPORT void off_pow( void )
{
}


#endif /* MTKBSP_MPCXPRESSO */
