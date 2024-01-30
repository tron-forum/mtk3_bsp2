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
 *    T-Monitor Communication low-level device driver (Nucleo STM32F446)
 */

#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>
#include <sysdepend/stm32_cube/halif.h>

#ifdef MTKBSP_NUCLEO_STM32F446
#if TM_COM_SERIAL_DEV

/* UART register definition (Use USART2) */
#define	UART_BASE		(0x40004400UL)		/* USART2 register base address */

#define UART_SR		(*(_UW*)(UART_BASE+0x0000))	/* Status register */
#define UART_DR		(*(_UW*)(UART_BASE+0x0004))	/* Data register */
#define UART_BRR	(*(_UW*)(UART_BASE+0x0008))	/* Baud Rate register */
#define UART_CR1	(*(_UW*)(UART_BASE+0x000C))	/* Control register-1 */
#define UART_CR2	(*(_UW*)(UART_BASE+0x0010))	/* Control register-2 */
#define UART_CR3	(*(_UW*)(UART_BASE+0x0014))	/* Control register-3 */
#define UART_GTPR	(*(_UW*)(UART_BASE+0x0018))	/* Guard time and prescaler register */

#define CR1_UE		(1<<13)				/* USART enable */
#define CR1_RE		(1<<2)				/* Receiver enable */
#define CR1_TE		(1<<3)				/* Transmitter enable */

#define SR_TXE		(1<<7)				/* Transmit data register empty */
#define SR_TC		(1<<6)				/* Transmission complete */
#define SR_RXNE		(1<<5)				/* Read data register not empty */

/* Communication speed */
#define UART_BAUD	(115200)			/* 115200 bps */

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while ((UART_SR & SR_TXE) == 0 );
		UART_DR = *b;
		while ((UART_SR & SR_TC) == 0 );
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while ( (UART_SR & SR_RXNE) == 0 );
		*buf = UART_DR & 0xff;
	}
}

EXPORT	void	tm_com_init(void)
{
	UW	pclk1;

	/* Initialize serial communication. Disable all interrupt. */
	UART_CR1 = 0;		/* 8bit, Non parity (Reset value) */
	UART_CR2 = 0;		/* Stop bit 1 (Reset value) */
	UART_CR3 = 0;		/* No hard flow control (Reset value) */

	/* Set baud rate */
	pclk1 = halif_get_pclk1();
	UART_BRR = (pclk1 + UART_BAUD/2)/UART_BAUD;

	UART_CR1 = CR1_UE | CR1_RE |CR1_TE;	/* Start UART */
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_NUCLEO_STM32F446 */
#endif /* USE_TMONITOR */