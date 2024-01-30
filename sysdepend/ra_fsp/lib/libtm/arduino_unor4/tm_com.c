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
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (Arduino UNO R4)
 */
#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>

#ifdef MTKBSP_ARDUINO_UNOR4
#if TM_COM_SERIAL_DEV

/* 
	Use SCI2(Serial communication interface 2) 
		P302: TXD2
		P301: RXD2
 */
/* register definition (SCI2) */
#define SCI2_BASE	(0x40070040)

#define	SCI2_SMR	(SCI2_BASE + 0x00)	// Serial mode register (UB)
#define SCI2_BRR	(SCI2_BASE + 0x01)	// Bit rate register (UB)
#define	SCI2_SCR	(SCI2_BASE + 0x02)	// Serial control register (UB)
#define	SCI2_TDR	(SCI2_BASE + 0x03)	// Transmit data register (UB)
#define	SCI2_SSR	(SCI2_BASE + 0x04)	// Serial status register (UB)
#define SCI2_RDR	(SCI2_BASE + 0x05)	// Receive data register (UB)
#define	SCI2_SCMR	(SCI2_BASE + 0x06)	// Smartcard mode register (UB)
#define SCI2_SEMR	(SCI2_BASE + 0x07)	// Serial Extend mode register (UB)
#define	SCI2_MDDR	(SCI2_BASE + 0x12)	// Modulation duty register (UB)

#define	SCR_TIE		(0x80)			// Transmit interrupt enable
#define	SCR_RIE		(0x40)			// Receive interrupt ebable
#define	SCR_TE		(0x20)			// Transmit enable
#define	SCR_RE		(0x10)			// Receive enable

#define	SSR_TDRE	(0x80)			// Transmit data empty
#define SSR_RDRF	(0x40)			// Receive data full
#define	SSR_ERR		(0x38)			// Error

#define MSTPCRB		(0x40047000)		// Module Stop register

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
#if 1
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while( (in_b(SCI2_SSR) & SSR_TDRE) == 0);
		out_b( SCI2_TDR, *b );
	}
#endif
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	UB	ssr;

	for( ; size > 0; size--, buf++ ){
		while(1) {
			ssr = in_b(SCI2_SSR);
			if( (ssr & SSR_RDRF) != 0) {
				*buf = in_b( SCI2_RDR );
				break;
			} else if( ssr & SSR_ERR ) {	/* check Communication error */
				out_b( SCI2_SSR, ssr & ~SSR_ERR);
			}
		}
	}
}

EXPORT	void	tm_com_init(void)
{
	out_w( MSTPCRB, in_w(MSTPCRB) & ~(1<<29));	// Enable SCI2

	out_b(SCI2_SCR, 0x00);			// TE = 0 RE = 0
	out_b(SCI2_SCMR, 0xF2);			// LSB first
	out_b(SCI2_SMR, 0x00);			// Data8 Stop1
	out_b(SCI2_BRR, 0x19);			// 115.2kbps
	out_b(SCI2_SEMR, 0xC0);
	out_b(SCI2_MDDR, 0x80);

	out_b(SCI2_SCR, SCR_TE | SCR_RE);	// TE = 1 RE = 1
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_EK_RA4M1 */
#endif /* USE_TMONITOR */