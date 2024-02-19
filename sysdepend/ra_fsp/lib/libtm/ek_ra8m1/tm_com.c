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

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (EK-RA8M1)
 */
#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>

#ifdef MTKBSP_EK_RA8M1
#if TM_COM_SERIAL_DEV

#include "bsp_api.h"

/* 
	Use SCI3(Serial communication interface 3) 
		P310: TXD
		P309: RXD
 */
/* register definition (SCI3) */
#define SCI3_BASE	(0x40358300)

#define SCI3_RDR	(SCI3_BASE + 0x00)	// Receive data register (UB)
#define	SCI3_TDR	(SCI3_BASE + 0x04)	// Transmit data register (UB)
#define	SCI3_CCR0	(SCI3_BASE + 0x08)	// Common Control register 0 (UW)
#define	SCI3_CCR1	(SCI3_BASE + 0xC)	// Common Control register 1 (UW)
#define	SCI3_CCR2	(SCI3_BASE + 0x10)	// Common Control register 2 (UW)
#define	SCI3_CCR3	(SCI3_BASE + 0x14)	// Common Control register 3 (UW)
#define	SCI3_CSR	(SCI3_BASE + 0x48)	// Common Status register (UW)
#define	SCI3_CFCLR	(SCI3_BASE + 0x68)	// Common Flag Clear register (UW)

#define	CCR0_TE		(1<<4)			// Transmit enable
#define	CCR0_RE		(1<<0)			// Receive enable

#define CSR_TDRE	(1<<29)			// Transmit data empty
#define CSR_RDRF	(1<<31)			// Receive data full
#define CSR_ERR		((1<<28)|(1<<27)|(1<<24))

#define MSTPCRB		(0x40203004)		// Module Stop register

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	while(size-- > 0) {
		while( (in_w(SCI3_CSR) & CSR_TDRE) == 0);
		out_b( SCI3_TDR, *buf++);
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	UW	csr;

	while(size-- > 0) {
		csr = in_w(SCI3_CSR);
		if( (csr & CSR_RDRF) != 0) {
			*buf++ = in_b( SCI3_RDR );
		} else if( csr & CSR_ERR ) {	/* check Communication error */
			out_w( SCI3_CFCLR, csr & CSR_ERR);
			size++;
		}
	}
}

EXPORT	void	tm_com_init(void)
{
	out_w( MSTPCRB, in_w(MSTPCRB) & ~(1<<28));	// Enable SCI3

	out_w(SCI3_CCR0, 0);				// TEIE, TIE, RIE, TE, RE = 0
	out_w(SCI3_CCR1, 0x00000010);			// Non parity
	out_w(SCI3_CCR2, 0x80004010);			// Set Set baud rate (115200 bps)
	out_w(SCI3_CCR0, CCR0_TE | CCR0_RE);		// TE = 1 RE = 1
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_EK_RA8M1 */
#endif /* USE_TMONITOR */