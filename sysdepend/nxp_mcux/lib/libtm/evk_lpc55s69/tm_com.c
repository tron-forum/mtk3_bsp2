/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (LPC55S69-EVK)
 */
#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>

#ifdef MTKBSP_EVK_LPC55S69
#if TM_COM_SERIAL_DEV

/* 
 */
/* register definition */

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
}

EXPORT	void	tm_com_init(void)
{
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_EVK_LPC55S69 */
#endif /* USE_TMONITOR */