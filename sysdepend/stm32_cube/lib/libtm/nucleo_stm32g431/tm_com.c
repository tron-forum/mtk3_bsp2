/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (Nucleo STM32G431)
 */

#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>
#include <sysdepend/stm32_cube/halif.h>

#ifdef MTKBSP_NUCLEO_STM32G431
#if TM_COM_SERIAL_DEV

/* UART register definition (Use LPUART1) */
#define	UART_BASE	(0x40008000UL)		/* LPUART1 register base address */

#define UART_CR1	(*(_UW*)(UART_BASE+0x0000))	/* Control register-1 */
#define UART_CR2	(*(_UW*)(UART_BASE+0x0004))	/* Control register-2 */
#define UART_CR3	(*(_UW*)(UART_BASE+0x0008))	/* Control register-3 */
#define UART_BRR	(*(_UW*)(UART_BASE+0x000C))	/* Baud Rate register */
#define UART_ISR	(*(_UW*)(UART_BASE+0x001C))	/* Status register */
#define UART_RDR	(*(_UW*)(UART_BASE+0x0024))	/* Receiver Data register */
#define UART_TDR	(*(_UW*)(UART_BASE+0x0028))	/* Transmitter Data register */
#define UART_PRESC	(*(_UW*)(UART_BASE+0x002C))	/* PRESCALER register */

#define CR1_UE		(1<<0)				/* UART enable */
#define CR1_RE		(1<<2)				/* Receiver enable */
#define CR1_TE		(1<<3)				/* Transmitter enable */

#define ISR_TXE		(1<<7)				/* Transmit data register empty */
#define ISR_TC		(1<<6)				/* Transmission complete */
#define ISR_RXNE	(1<<5)				/* Read data register not empty */

/* Communication speed */
#define UART_BAUD	(115200)			/* 115200 bps */

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while ((UART_ISR & ISR_TXE) == 0 );
		UART_TDR = *b;
		while ((UART_ISR & ISR_TC) == 0 );
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while ( (UART_ISR & ISR_RXNE) == 0 );
		*buf = UART_RDR & 0xff;
	}
}

EXPORT	void	tm_com_init(void)
{
	const	UW presc[] = { 1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256 };
	UD	brr;
	
	/* Initialize serial communication. Disable all interrupt. */
	UART_CR1 = 0;		/* 8bit, Non parity (Reset value) */
	UART_CR2 = 0;		/* Stop bit 1 (Reset value) */
	UART_CR3 = 0;		/* No hard flow control (Reset value) */

	/* Set baud rate */
	brr = (UD)(halif_get_pclk1()/presc[UART_PRESC&0x0F]);
	brr = (brr*256 + UART_BAUD/2)/UART_BAUD;
	UART_BRR = (UW)brr;

	UART_CR1 = CR1_UE | CR1_RE |CR1_TE;	/* Start UART */
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_NUCLEO_STM32G431 */
#endif /* USE_TMONITOR */