/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef MTKBSP_EVK_XMC7200

/*
 *	hw_setting.c (XMC7200D-E272K8384)
 *	startup / shoutdown processing for hardware
 */

#include <kernel.h>
#include <tm/tmonitor.h>

#include "sysdepend.h"

#include <cybsp.h>

#define UNUSED_INT	(0xFFU)

typedef struct irq_table
{
	UINT intno;
	INT level;
} IRQ_TABLE;

LOCAL const IRQ_TABLE nvic_table[CORE_EXT_INTVEC] =
{
	/* Interrupt priorities 1 and 7 are used by the OS and cannot be set. */
	{0U, 2U},	/* IRQ 0 */
	{1U, 3U},	/* IRQ 1 */
	{2U, 4U},	/* IRQ 2 */
	{3U, 5U},	/* IRQ 3 */
	{4U, 6U},	/* IRQ 4 */
	{UNUSED_INT, 7U},	/* IRQ 5 */
	{UNUSED_INT, 8U},	/* IRQ 6 */
	{UNUSED_INT, 9U},	/* IRQ 7 */
};

IMPORT void EnableInt_nvic( UINT intno, INT level );

LOCAL void enable_irq(void)
{
	UB loop_cnt;

	for(loop_cnt = 0U; loop_cnt < CORE_EXT_INTVEC; loop_cnt++)
	{
		if (nvic_table[loop_cnt].intno != UNUSED_INT)
		{
			EnableInt_nvic(nvic_table[loop_cnt].intno, nvic_table[loop_cnt].level);
		}
	}
}

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	/* Enable IRQ Register */
	enable_irq();
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* MTKBSP_EVK_XMC7200 */
