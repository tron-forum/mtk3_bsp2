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

/*
 *	sysdepend.h (ARMv7-M)
 *	System-Dependent local defined
 */

#ifndef _MTKBSP_SYSDEPEND_CPU_CORE_SYSDEPEND_
#define _MTKBSP_SYSDEPEND_CPU_CORE_SYSDEPEND_

/*
 * Dispatcher (dispatch.S)
 */
IMPORT void knl_dispatch_entry(void);		/* dispatch entry */
IMPORT void knl_dispatch_to_schedtsk(void);	/* force dispatch */

/*
 * Interrupt Control (interrupt.c)
 */
IMPORT UW knl_exctbl[];		/* Exception handler table (RAM) */
IMPORT UW *knl_exctbl_o;		// Exception handler table (Origin)
IMPORT void knl_systim_inthdr(void);	/* System-timer Interrupt handler */


/*
 * Task context block
 */
typedef struct {
	void	*ssp;		/* System stack pointer */
} CTXB;

/*
 *	Control register operation
 */

Inline UW knl_get_ipsr(void)
{
	UW	ipsr;
	Asm("mrs %0, ipsr": "=r"(ipsr));
	return ipsr;
}

Inline UW knl_get_xpsr(void)
{
	UW	xpsr;
	Asm("mrs %0, psr": "=r"(xpsr));
	return xpsr;
}

Inline UW knl_get_primask(void)
{
	UW	primask;
	Asm("mrs %0, primask": "=r"(primask));
	return primask;
}


#endif /* _MTKBSP_SYSDEPEND_CPU_CORE_SYSDEPEND_ */
