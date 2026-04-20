/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (EK-RA8P1)
 */
#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>

#ifdef MTKBSP_EK_RA8P1
#if TM_COM_SERIAL_DEV

#include "bsp_api.h"

/* 
	Use SCI8(Serial communication interface 8) 
		PD02: TXD8
		PD03: RXD8
 */
/* register definition (SCI8) */
#define SCI8_BASE	(0x40358800)

#define SCI8_RDR	(SCI8_BASE + 0x00)	// Receive data register (UB)
#define	SCI8_TDR	(SCI8_BASE + 0x04)	// Transmit data register (UB)
#define	SCI8_CCR0	(SCI8_BASE + 0x08)	// Common Control register 0 (UW)
#define	SCI8_CCR1	(SCI8_BASE + 0xC)	// Common Control register 1 (UW)
#define	SCI8_CCR2	(SCI8_BASE + 0x10)	// Common Control register 2 (UW)
#define	SCI8_CCR3	(SCI8_BASE + 0x14)	// Common Control register 3 (UW)
#define	SCI8_CSR	(SCI8_BASE + 0x48)	// Common Status register (UW)
#define	SCI8_CFCLR	(SCI8_BASE + 0x68)	// Common Flag Clear register (UW)

#define	CCR0_TE		(1<<4)			// Transmit enable
#define	CCR0_RE		(1<<0)			// Receive enable

#define CSR_TDRE	(1<<29)			// Transmit data empty
#define CSR_RDRF	(1<<31)			// Receive data full
#define CSR_ERR		((1<<28)|(1<<27)|(1<<24))

#define MSTPCRB		(0x40203004)		// Module Stop register

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	while(size-- > 0) {
		while( (in_w(SCI8_CSR) & CSR_TDRE) == 0);
		out_b( SCI8_TDR, *buf++);
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	UW	csr;

	while(size-- > 0) {
		csr = in_w(SCI8_CSR);
		if( (csr & CSR_RDRF) != 0) {
			*buf++ = in_b( SCI8_RDR );
		} else if( csr & CSR_ERR ) {	/* check Communication error */
			out_w( SCI8_CFCLR, csr & CSR_ERR);
			size++;
		}
	}
}

EXPORT	void	tm_com_init(void)
{
	out_w( MSTPCRB, in_w(MSTPCRB) & ~(1<<23));	// Enable SCI8

	out_w(SCI8_CCR0, 0);				// TEIE, TIE, RIE, TE, RE = 0
	out_w(SCI8_CCR1, 0x00000010);			// Non parity
	out_w(SCI8_CCR2, 0x80004010);			// Set Set baud rate (115200 bps)
	out_w(SCI8_CCR0, CCR0_TE | CCR0_RE);		// TE = 1 RE = 1
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_EK_RA8P1 */
#endif /* USE_TMONITOR */