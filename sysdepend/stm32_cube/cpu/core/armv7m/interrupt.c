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
#if defined(MTKBSP_STM32CUBE) && defined(MTKBSP_CPU_CORE_ARMV7M)
/*
 *	interrupt.c (ARMv7-M)
 *	Interrupt control
 */

#include <tk/tkernel.h>
#include <kernel.h>
#include "sysdepend.h"
#include "cpu_status.h"

/* HLL Interrupt Handler Table */
LOCAL UW hllint_tbl[N_INTVEC];

/* ------------------------------------------------------------------------ */
/*
 * HLL(High level programming language) Interrupt Handler
 */
EXPORT void knl_hll_inthdr(void)
{
	FP	inthdr;
	UW	intno;

	ENTER_TASK_INDEPENDENT;

	intno	= knl_get_ipsr() - 16;
	inthdr	= (FP)hllint_tbl[intno];

	(*inthdr)(intno);

	LEAVE_TASK_INDEPENDENT;
}

/* ------------------------------------------------------------------------ */
/*
 * System-timer Interrupt handler
 */
EXPORT void knl_systim_inthdr(void)
{
	ENTER_TASK_INDEPENDENT;

	knl_timer_handler();

	LEAVE_TASK_INDEPENDENT;
}

/* ----------------------------------------------------------------------- */
/*
 * Set interrupt handler (Used in tk_def_int())
 */
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
	volatile FP	*intvet;

	if(inthdr != NULL) {
		if ( (intatr & TA_HLNG) != 0 ) {
			hllint_tbl[intno] = (UW)inthdr;
			inthdr = knl_hll_inthdr;
		}		
	} else 	{	/* Clear interrupt handler */
		inthdr = (FP)knl_exctbl_o[N_SYSVEC + intno];
	}
	intvet = (FP*)(knl_exctbl + N_SYSVEC);
	intvet[intno] = inthdr;

	return E_OK;
}

/* ----------------------------------------------------------------------- */
/*
 * Return interrupt handler (Used in tk_ret_int())
 */
EXPORT void knl_return_inthdr(void)
{
	/* No processing in ARM. */
	return;
}

/* ------------------------------------------------------------------------ */
/*
 * Interrupt initialize
 */
EXPORT ER knl_init_interrupt( void )
{
	/* Set Exception handler */
	knl_exctbl[14]	= (UW)knl_dispatch_entry;
	knl_exctbl[15]	= (UW)knl_systim_inthdr;

	return E_OK;
}

#endif	/* defined(MTKBSP_STM32CUBE) && defined(MTKBSP_CPU_CORE_ARMV7M) */