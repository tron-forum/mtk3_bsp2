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

#include <sys/machine.h>
#ifdef MTKBSP_CPU_XMC7200

/*
 *	int_xmc7200.c
 *
 *	Interrupt controller (XMC7200)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include <sysdepend/xmc_mtb/lib/libtk/cpu/core/armv7m/int_armv7m.h>

#include <cy_sysint.h>

#define UNUSED_IRQNO	(0xFFU)

typedef struct ext_int_table
{
	INT level;	/* Interrupt level */
	UINT irqno;	/* IRQ No */
} EXT_INT_TABLE;

LOCAL const EXT_INT_TABLE ext_int_table[CORE_EXT_INTVEC] =
{
	/* Interrupt priorities 1 and 7 are used by the OS and cannot be set. */
	/* If level 2 to 6 is selected, IRQ0 to 4 interrupts are enabled. */
	{0U, UNUSED_IRQNO},	/* level 0 */
	{1U, UNUSED_IRQNO},	/* level 1 */
	{2U, 0U},	/* level 2 */
	{3U, 1U},	/* level 3 */
	{4U, 2U},	/* level 4 */
	{5U, 3U},	/* level 5 */
	{6U, 4U},	/* level 6 */
	{7U, UNUSED_IRQNO},	/* level 7 */
};

/*----------------------------------------------------------------------*/
/*
 * Interrupt control API
 * 
 */
/*
 * Enable interrupt 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	UINT ext_intno;

	/* External interrupt */
	/* External Interrupts number is 0-566 */
	if (intno < (N_INTVEC - CORE_SOFT_INTVEC))
	{
		/* Decide which interrupt number to assign with level */
		ext_intno = ext_int_table[level].irqno;
		if (ext_intno == UNUSED_IRQNO)
		{
			return;
		}
		/* This field specifies to which CPU interrupt the system interrupt is mapped.
		   E.g., if CPU_INT_IDX is '6', the system interrupt is mapped to CPU interrupt '6'. */
		Cy_SysInt_SetInterruptSource(ext_intno, intno);
	}
	else
	{
		/* Software interrupt */
		/* The interrupt number corresponding to software interrupt number 567-574 is IRQ8-15. */
		ext_intno = (intno + CORE_EXT_INTVEC + CORE_SOFT_INTVEC - N_INTVEC);
		EnableInt_nvic(ext_intno, level);
	}
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	/* External interrupt */
	/* External Interrupts number is 0-566 */
	if (intno < (N_INTVEC - CORE_SOFT_INTVEC))
	{
		Cy_SysInt_DisableSystemInt(intno);
	}
	else
	{
		/* Software interrupt */
		/* The interrupt number corresponding to software interrupt number 567-574 is IRQ8-15. */
		DisableInt_nvic(intno + CORE_EXT_INTVEC + CORE_SOFT_INTVEC - N_INTVEC);
	}
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	/* External interrupt */
	/* External Interrupts number is 0-566 */
	if (intno < (N_INTVEC - CORE_SOFT_INTVEC))
	{
		/* No opetarion. */
	}
	else
	{
		/* Software interrupt */
		/* The interrupt number corresponding to software interrupt number 567-574 is IRQ8-15. */
		ClearInt_nvic(intno + CORE_EXT_INTVEC + CORE_SOFT_INTVEC - N_INTVEC);		/* Un-pends the associated interrupt */
    }
}

/*
 * Issue EOI to interrupt controller
 */
EXPORT void EndOfInt(UINT intno)
{
	/* No opetarion. */
}

/*
 * Check active state
 */
EXPORT BOOL CheckInt( UINT intno )
{
	/* External interrupt */
	/* External Interrupts number is 0-566 */
	if (intno < (N_INTVEC - CORE_SOFT_INTVEC))
	{
		/* No opetarion. */
		return 0;
	}
	else
	{
		/* Software interrupt */
		/* The interrupt number corresponding to software interrupt number 567-574 is IRQ8-15. */
		return  CheckInt_nvic(intno + CORE_EXT_INTVEC + CORE_SOFT_INTVEC - N_INTVEC);
	}
}

/*
 * Set interrupt mode
 */
#if TK_SUPPORT_INTMODE
EXPORT void SetIntMode(UINT intno, UINT mode)
{
}
#endif

#endif /* MTKBSP_CPU_XMC7200 */
