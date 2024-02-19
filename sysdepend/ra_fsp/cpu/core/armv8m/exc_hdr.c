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
#if defined(MTKBSP_RAFSP) && defined(MTKBSP_CPU_CORE_ARMV8M)
/*
 *	exc_hdr.c (ARMv8-M)
 *	Exception handler
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <kernel.h>
#include "sysdepend.h"
#include "cpu_status.h"

#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	#define EXCEPTION_DBG_MSG(a)	tm_putstring((UB*)a)
#else
	#define EXCEPTION_DBG_MSG(a)
#endif

/*
 * NMI handler
 */
WEAK_FUNC EXPORT void knl_nmi_handler(void)
{
	EXCEPTION_DBG_MSG("NMI\n");
	while(1);
}

/*
 * Hard fault handler
 */
//WEAK_FUNC EXPORT void knl_hardfault_handler(void)
void knl_hardfault_handler(void)
{
#if (USE_EXCEPTION_DBG_MSG  && USE_TMONITOR)

	UW	hfsr, cfsr;
	ID	ctskid;

	hfsr	= *(_UW *)SCB_HFSR;
	if(knl_ctxtsk != NULL) {
		ctskid = knl_ctxtsk->tskid;
	} else {
		ctskid = 0;
	}

	if(hfsr & 0x40000000) {
		cfsr = *(_UW*)SCB_CFSR;
		tm_printf((UB*)"*** Hard fault ***  ctxtsk:%d  HFSR:%x  CFSR:%x\n", ctskid, hfsr, cfsr);
	} else {
		tm_printf((UB*)"*** Hard fault ***  ctxtsk:%d  HFSR:%x%x\n", ctskid, hfsr);
	}
#endif
	while(1);
}

/*
 * MPU Fault Handler
 */
WEAK_FUNC EXPORT void knl_memmanage_handler(void)
{
	EXCEPTION_DBG_MSG("MPU Fault\n");
	while(1);
}

/* 
 * Bus Fault Handler
 */
WEAK_FUNC EXPORT void knl_busfault_handler(void)
{
	EXCEPTION_DBG_MSG("Bus Fault\n");
	while(1);
}

/*
 * Usage Fault Handler
 */
//WEAK_FUNC EXPORT void knl_usagefault_handler(void)
EXPORT void knl_usagefault_handler(void)
{
	EXCEPTION_DBG_MSG("Usage Fault\n");
	while(1);
}

/*
 * Svcall
 */
WEAK_FUNC EXPORT void knl_svcall_handler(void)
{
	EXCEPTION_DBG_MSG("SVCall\n");
	while(1);
}

/* 
 * Debug Monitor
 */
WEAK_FUNC EXPORT void knl_debugmon_handler(void)
{
	EXCEPTION_DBG_MSG("Debug Monitor\n");
	while(1);
}

/*
 * Default Handler
 */
WEAK_FUNC EXPORT void knl_default_handler(void)
{
#if (USE_EXCEPTION_DBG_MSG  && USE_TMONITOR)
	INT	i;
	_UW	*icpr;

	icpr = (_UW*)NVIC_ICPR_BASE;

	EXCEPTION_DBG_MSG("Undefine Exceptio ICPR: ");
	for(i=0; i < 8; i++) {
		tm_printf((UB*)"%x ", *icpr++);
	}
	EXCEPTION_DBG_MSG("\n");
#endif
	while(1);
}

#endif /* defined(MTKBSP_RAFSP) && defined(MTKBSP_CPU_CORE_ARMV8M) */
