/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/08.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef MTKBSP_CPU_STM32H5

/*
 *	int_stm32h5.c
 *
 *	Interrupt controller (STM32CUBE STM32H5 )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include <sysdepend/stm32_cube/lib/libtk/cpu/core/armv8m/int_armv8m.h>

/*----------------------------------------------------------------------*/
/*
 * EXTI (Extended interrupt controller) functions
 * 
 */
LOCAL void EnableInt_exti( UINT intno, INT level )
{
	*(_UW*)EXTI_IMR(intno>>5) |= (UW)(1<<(intno%32));
}

LOCAL void DisableInt_exti( UINT intno )
{
	*(_UW*)EXTI_IMR(intno>>5) &= ~(UW)(1<<(intno%32));
}

LOCAL void ClearInt_exti( UINT intno )
{
	*(_UW*)EXTI_RPR(intno>>5) |= (UW)(1<<(intno%32));
	*(_UW*)EXTI_FPR(intno>>5) |= (UW)(1<<(intno%32));
}

LOCAL BOOL CheckInt_exti( UINT intno )
{
	UW	pif;

	pif = *(_UW*)EXTI_RPR(intno>>5) & (UW)(1<<(intno%32));
	pif |= *(_UW*)EXTI_FPR(intno>>5) & (UW)(1<<(intno%32));

	return pif?TRUE:FALSE;
}

LOCAL void SetIntMode_exti(UINT intno, UINT mode)
{
	if(mode & IM_HI) {
		*(_UW*)EXTI_RTSR(intno>>5) |= (UW)(1<<(intno%32));
	} else {
		*(_UW*)EXTI_RTSR(intno>>5) &= ~(UW)(1<<(intno%32));
	}

	if(mode & IM_LOW) {
		*(_UW*)EXTI_FTSR(intno>>5) |= (UW)(1<<(intno%32));
	} else {
		*(_UW*)EXTI_FTSR(intno>>5) &= ~(UW)(1<<(intno%32));
	}
}

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
	if( intno <= MAX_NVIC_INTNO) {
		EnableInt_nvic( intno, level);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		EnableInt_exti( intno - MIN_EXTI_INTNO, level);
	}
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	if( intno <= MAX_NVIC_INTNO) {
		DisableInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		DisableInt_exti( intno - MIN_EXTI_INTNO);
	}
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	if( intno <= MAX_NVIC_INTNO) {
		ClearInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		ClearInt_exti( intno - MIN_EXTI_INTNO);
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
	BOOL rtncd;

	if( intno <= MAX_NVIC_INTNO) {
		rtncd = CheckInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		rtncd = CheckInt_exti( intno - MIN_EXTI_INTNO);
	} else {
		rtncd = FALSE;
	}
	return rtncd;
}

/*
 * Set interrupt mode
 */
EXPORT void SetIntMode(UINT intno, UINT mode)
{
	if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		SetIntMode_exti( intno - MIN_EXTI_INTNO, mode);
	}
}

#endif /* MTKBSP_CPU_STM32H5 */
