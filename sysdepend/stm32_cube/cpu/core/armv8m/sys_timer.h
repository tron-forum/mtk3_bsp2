/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sys_timer.h (ARMv8-M)
 *	Hardware-Dependent System Timer (SysTick) Processing
 */

#ifndef _MTKBSP_SYSDEPEND_CPU_CORE_SYSTIMER_
#define _MTKBSP_SYSDEPEND_CPU_CORE_SYSTIMER_

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void knl_start_hw_timer( void )
{
	UINT	n, imask;

	DI(imask);

	/* Set System timer CLK source to Core, Systick exception enable */
	out_w(SYST_CSR, 0x00000006);

	/* Set counter: TMCLK(MHz) */
	n = (UINT)(TIMER_PERIOD * TMCLK_KHz - 1);
	out_w(SYST_RVR, n);
	out_w(SYST_CVR, 0);

	/* Start timer count */
	out_w(SYST_CSR, 0x00000007);

	EI(imask);
}

/*
 * Clear timer interrupt
 *	Clear the timer interrupt request. Depending on the type of
 *	hardware, there are two timings for clearing: at the beginning
 *	and the end of the interrupt handler.
 *	'clear_hw_timer_interrupt()' is called at the beginning of the
 *	timer interrupt handler.
 *	'end_of_hw_timer_interrupt()' is called at the end of the timer
 *	interrupt handler.
 *	Use either or both according to hardware.
 */
Inline void knl_clear_hw_timer_interrupt( void )
{
	(void)in_w(SYST_CSR);			/* Clear COUNTFLAG */
	out_w(SCB_ICSR, ICSR_PENDSTCLR);
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* No processing */
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	/* Timer interrupt disable */
	out_w(SYST_CSR, 0x00000000);
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < TIMER_PERIOD * 2
 */
Inline UW knl_get_hw_timer_nsec( void )
{
	UW	ofs, max, unf;
	UINT	imsk;

	DI(imsk);
	max = in_w(SYST_RVR);			/* Setting count */
	unf = in_w(SYST_CSR) & 0x10000;		/* COUNTFLAG */
	ofs = in_w(SYST_CVR) & 0x00ffffff;	/* Current Remained count */
	if ( unf == 0 ) {		/* Reload not occurred */
		unf = in_w(SYST_CSR) & 0x10000;	/* Check COUNTFLAG again */
		if (unf != 0) {		/* Reload occurred */
			ofs = in_w(SYST_CVR) & 0x00ffffff;
		}
	}
	EI(imsk);
	ofs = max - ofs;			/* Elapsed count */
	if ( unf != 0 ) ofs += max + 1;	/* Reload occured, Adjust */

	return  (UW) ( ( (D)ofs * 1000000 ) / TMCLK_KHz );
}

#endif /* _MTKBSP_SYSDEPEND_CPU_CORE_SYSTIMER_ */
