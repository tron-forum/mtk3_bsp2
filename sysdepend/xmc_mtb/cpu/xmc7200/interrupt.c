/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#if defined(MTKBSP_MODUSTOOLBOX) && defined(MTKBSP_CPU_XMC7200)
/*
 *	interrupt.c (XMC7200)
 *	Interrupt control
 */
#include <tk/tkernel.h>
#include <kernel.h>
#include "sysdepend.h"
#include "cpu_status.h"

#include <sysdepend/xmc_mtb/lib/libtk/cpu/core/armv7m/int_armv7m.h>

#include <cy_syslib.h>

#if !USE_STATIC_IVT
/* ------------------------------------------------------------------------ */
/*
 * Exception handler table (RAM)
 */
#if defined (MTKBSP_CPU_XMC7200)
EXPORT UW exchdr_tbl[N_SYSVEC + CORE_EXT_INTVEC + CORE_SOFT_INTVEC] __attribute__ ((section (".data_vector")));
#else
EXPORT UW exchdr_tbl[N_SYSVEC + N_INTVEC] __attribute__ ((section (".data_vector")));
#endif

#endif

/* ------------------------------------------------------------------------ */
/*
 * HLL(High level programming language) Interrupt Handler
 */
#if defined (MTKBSP_CPU_XMC7200)
Noinit(EXPORT FP knl_inthdr_tbl[N_INTVEC]);	/* HLL Interrupt Handler Table */
#else
Noinit(LOCAL FP knl_inthdr_tbl[N_INTVEC]);	/* HLL Interrupt Handler Table */
#endif

IMPORT cy_israddress Cy_SystemIrqUserTable[];

/*
 * The handler for Software interrupt.
 * The system interrupt mapped to CPU interrupt will be fetched and executed
 */
LOCAL void CM7_SoftIntr_Handler(UINT intno)
{
	UW	system_int_idx;
	FP	inthdr;

	ENTER_TASK_INDEPENDENT;

	/* The interrupt number corresponding to software interrupt number 567-574 is IRQ8-15. */
	system_int_idx = (N_INTVEC - (CORE_EXT_INTVEC + CORE_SOFT_INTVEC) + intno);

	inthdr = knl_inthdr_tbl[system_int_idx];
	inthdr(system_int_idx); // jump to system interrupt handler

	ClearInt_nvic(intno);

	LEAVE_TASK_INDEPENDENT;
}

/*
 * The Handler is called when the CPU interrupt8 occurs.
 */
LOCAL void CM7_CpuIntr8_Handler(void)
{
	CM7_SoftIntr_Handler(8);
}

/*
 * The Handler is called when the CPU interrupt9 occurs.
 */
LOCAL void CM7_CpuIntr9_Handler(void)
{
	CM7_SoftIntr_Handler(9);
}

/*
 * The Handler is called when the CPU interrupt10 occurs.
 */
LOCAL void CM7_CpuIntr10_Handler(void)
{
	CM7_SoftIntr_Handler(10);
}

/*
 * The Handler is called when the CPU interrupt11 occurs.
 */
LOCAL void CM7_CpuIntr11_Handler(void)
{
	CM7_SoftIntr_Handler(11);
}

/*
 * The Handler is called when the CPU interrupt12 occurs.
 */
LOCAL void CM7_CpuIntr12_Handler(void)
{
	CM7_SoftIntr_Handler(12);
}

/*
 * The Handler is called when the CPU interrupt13 occurs.
 */
LOCAL void CM7_CpuIntr13_Handler(void)
{
	CM7_SoftIntr_Handler(13);
}

/*
 * The Handler is called when the CPU interrupt14 occurs.
 */
LOCAL void CM7_CpuIntr14_Handler(void)
{
	CM7_SoftIntr_Handler(14);
}

/*
 * The Handler is called when the CPU interrupt15 occurs.
 */
LOCAL void CM7_CpuIntr15_Handler(void)
{
	CM7_SoftIntr_Handler(15);
}

EXPORT void knl_hll_inthdr(void)
{
	FP	inthdr;
	UW	intno;
	UW	system_int_idx;

	ENTER_TASK_INDEPENDENT;

	intno	= knl_get_ipsr() - 16;
	
	/* Check module interrupts with CPUSS_CM7_X_INT_STATUS of XMC7200. */
	if((CPUSS_CM7_0_INT_STATUS[intno] & SYSTEM_INT_VALID) == SYSTEM_INT_VALID)
	{
		system_int_idx = (CPUSS_CM7_0_INT_STATUS[intno] & SYSTEM_INT_IDX_MASK);
		inthdr = knl_inthdr_tbl[system_int_idx];
		inthdr(system_int_idx); // jump to system interrupt handler
	}

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
	if(inthdr != NULL) {
		if ( (intatr & TA_HLNG) != 0 ) {
			knl_inthdr_tbl[intno] = inthdr;
			inthdr = knl_hll_inthdr;
		}		
	} else 	{	/* Clear interrupt handler */
		inthdr = Default_Handler;
	}
	Cy_SystemIrqUserTable[intno] = inthdr;

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
	
	knl_exctbl[24] = (UW)CM7_CpuIntr8_Handler;
	knl_exctbl[25] = (UW)CM7_CpuIntr9_Handler;
	knl_exctbl[26] = (UW)CM7_CpuIntr10_Handler;
	knl_exctbl[27] = (UW)CM7_CpuIntr11_Handler;
	knl_exctbl[28] = (UW)CM7_CpuIntr12_Handler;
	knl_exctbl[29] = (UW)CM7_CpuIntr13_Handler;
	knl_exctbl[30] = (UW)CM7_CpuIntr14_Handler;
	knl_exctbl[31] = (UW)CM7_CpuIntr15_Handler;

	return E_OK;
}


#endif	/* defined(MTKBSP_MODUSTOOLBOX) && defined(MTKBSP_CPU_CORE_ARMV7M) */