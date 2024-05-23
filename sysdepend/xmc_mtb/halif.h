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
 *	halif.h (ModusToolBox)
 *	HAL I/F
 */
#ifndef _MTKBSP_SYSDEPEND_HALIF_
#define _MTKBSP_SYSDEPEND_HALIF_
#include <cy_sysclk.h>
#include <stdint.h>


/* Get clock frequency (CMSIS wrapper function)*/

Inline uint32_t halif_get_sysclk(void)
{
	return Cy_SysClk_ClkFastSrcGetFrequency(0);
}

#endif	/* _MTKBSP_SYSDEPEND_HALIF_ */