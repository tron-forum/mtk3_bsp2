/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2023/11/22
 * 
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (EVK_XMC7200)
 */

#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>
#include <cy_sysclk.h>
#include <cy_gpio.h>

#ifdef MTKBSP_EVK_XMC7200
#if TM_COM_SERIAL_DEV
#define UART_CH3	(3)
#define TERM_PORT	(UART_CH3)
#define MAX_UART_NO	(11)		/* = 1 - 11 : Number of UART ports */

#define UART_BASE	(0x40600000ul)
#define UART0_BASE	(0x40600000ul)
#define UART1_BASE	(0x40610000ul)
#define UART2_BASE	(0x40620000ul)
#define UART3_BASE	(0x40630000ul)
#define UART4_BASE	(0x40640000ul)
#define UART5_BASE	(0x40650000ul)
#define UART6_BASE	(0x40660000ul)
#define UART7_BASE	(0x40670000ul)
#define UART8_BASE	(0x40680000ul)
#define UART9_BASE	(0x40690000ul)
#define UART10_BASE	(0x406A0000ul)

#define SCBxCTRL(p)		(_UW*)(uart_reg_base[p]+0x0000ul)	/* Generic control register */
#define SCBxUART_CTRL(p)	(_UW*)(uart_reg_base[p]+0x0040ul)	/* UART control register */
#define SCBxUART_TX_CTRL(p)	(_UW*)(uart_reg_base[p]+0x0044ul)	/* UART transmitter control register */
#define SCBxUART_RX_CTRL(p)	(_UW*)(uart_reg_base[p]+0x0048ul)	/* UART receiver control register */
#define SCBxUART_RX_STATUS(p)	(_UW*)(uart_reg_base[p]+0x004Cul)	/* UART receiver status register */
#define SCBxUART_FLOW_CTRL(p)	(_UW*)(uart_reg_base[p]+0x0050ul)	/* UART flow control register */
#define SCBxTX_CTRL(p)		(_UW*)(uart_reg_base[p]+0x00200ul)	/* transmitter control register */
#define SCBxTX_FIFO_CTRL(p)	(_UW*)(uart_reg_base[p]+0x00204ul)	/* transmitter FIFO control register */
#define SCBxTX_FIFO_STATUS(p)	(_UW*)(uart_reg_base[p]+0x00208ul)	/* transmitter FIFO status register */
#define SCBxTX_FIFO_WR(p)	(_UW*)(uart_reg_base[p]+0x00240ul)	/* transmitter FIFO write register */
#define SCBxRX_CTRL(p)		(_UW*)(uart_reg_base[p]+0x00300ul)	/* Receiver control register */
#define SCBxRX_FIFO_CTRL(p)	(_UW*)(uart_reg_base[p]+0x00304ul)	/* Receiver FIFO control register */
#define SCBxRX_FIFO_STATUS(p)	(_UW*)(uart_reg_base[p]+0x00308ul)	/* Receiver FIFO status register */
#define SCBxRX_FIFO_RD(p)	(_UW*)(uart_reg_base[p]+0x00340ul)	/* Receiver FIFO read register */
#define SCBxINTR_TX_MASK(p)	(_UW*)(uart_reg_base[p]+0x00F88ul)	/* Transmitter interrupt mask register */
#define SCBxINTR_RX(p)		(_UW*)(uart_reg_base[p]+0x00FC0ul)	/* Receiver interrupt request register */
#define SCBxINTR_RX_MASK(p)	(_UW*)(uart_reg_base[p]+0x00FC8ul)	/* Receiver interrupt mask register */

#define CTRL_OVS(x)		((x) << 0)
#define CTRL_WIDTH_BYTE		(0U << 14)	/* 8-bit FIFO data elements. */
#define CTRL_WIDTH_HFWORD	(1U << 14)	/* 16-bit FIFO data elements. */
#define CTRL_WIDTH_WORD		(2U << 14)	/* 32-bit FIFO data elements. */
#define CTRL_MODE_UART		(2U << 24)	/* UART mode. */
#define CTRL_ENABLED		(1U << 31)	/* IP enabled */
#define CTRL_WIDTH_MASK		(0x0000C000)

#define UART_CTRL_MODE_STD		(0U << 24)	/* Standard UART mode. */

#define UART_RX_CTRL_STOP_BIT(x)	((x) << 0)	/* stop bits. */
							/* STOP_BITS + 1 is the duration of the stopperiod in terms of halve bit periods. */
#define UART_RX_CTRL_STOP_BIT_MASK	(0x00000007)
#define UART_RX_CTRL_EVEN_PARITY	(0U << 4)	/* even parity */
#define UART_RX_CTRL_ODD_PARITY		(1U << 4)	/* odd parity */
#define UART_RX_CTRL_PARITY_DISABLE	(0U << 5)	/* Parity checking disabled */
#define UART_RX_CTRL_PARITY_ENABLE	(1U << 5)	/* Parity checking enabled */
#define UART_RX_CTRL_PARITY_MASK	(0x00000030)
#define UART_RX_CTRL_BREAK_WIDTH(x)	((x) << 16)	/* Break width.  */

#define RX_CTRL_DATA_WIDTH(x)		((x) << 0)	/* Dataframe width. DATA_WIDTH + 1. the valid range is [3, 8]. */
#define RX_CTRL_LSB_FIRST		(0U << 7)	/* LSB first */
#define RX_CTRL_MSB_FIRST		(1U << 7)	/* MSB first */

#define RX_FIFO_CTRL_LEVEL(x)		((x) << 0)	/* Trigger level. */

#define UART_TX_CTRL_STOP_BIT(x)	((x) << 0)	/* stop bits. */
							/* STOP_BITS + 1 is the duration of the stopperiod in terms of halve bit periods. */
#define UART_TX_CTRL_EVEN_PARITY	(0U << 4)	/* even parity */
#define UART_TX_CTRL_ODD_PARITY		(1U << 4)	/* odd parity */
#define UART_TX_CTRL_PARITY_DISABLE	(0U << 5)	/* Parity checking disabled */
#define UART_TX_CTRL_PARITY_ENABLE	(1U << 5)	/* Parity checking enabled */

#define TX_CTRL_DATA_WIDTH(x)		((x) << 0)	/* Dataframe width. DATA_WIDTH + 1. the valid range is [3, 8]. */
#define TX_CTRL_LSB_FIRST		(0U << 7)	/* LSB first */
#define TX_CTRL_MSB_FIRST		(1U << 7)	/* MSB first */

#define TX_FIFO_CTRL_LEVEL(x)		((x) << 0)	/* Trigger level. */

#define SCB_INTR_TX_EMPTY		(0x00000010)
#define SCB_INTR_TX_NOT_FULL		(0x00000002)
#define SCB_INTR_RX_NOT_EMPTY		(0x00000004)

#define SCB_RX_FIFO_STATUS_USED		(0x000000FF)
#define SCB_TX_FIFO_STATUS_USED		(0x000001FF)

#define UART_FLOW_CTRL_TRIGGER_LEVEL(x)	((x) << 0)	/* Trigger level.  */
#define UART_FLOW_CTRL_TRIGGER_LEVEL_MASK  (0x000000FF)
#define UART_FLOW_CTRL_RTS_LOW		(0U << 16)	/* Polarity of the RTS output signal 'uart_rts_out' 0:Low Active */
#define UART_FLOW_CTRL_RTS_HIGH		(1U << 16)	/* Polarity of the RTS output signal 'uart_rts_out' 1:High Active */
#define UART_FLOW_CTRL_CTS_LOW		(0U << 24)	/* Polarity of the CTS input signal 'uart_cts_in' 0:Low Active */
#define UART_FLOW_CTRL_CTS_HIGH		(1U << 24)	/* Polarity of the RTS output signal 'uart_rts_out' 1:High Active */
#define UART_FLOW_CTRL_CTS_DISABLE	(0U << 25)	/* Disable use of CTS input signal 'uart_cts_in' by the UART transmitter: */
#define UART_FLOW_CTRL_CTS_ENABLE	(1U << 25)	/* Enable use of CTS input signal 'uart_cts_in' by the UART transmitter: */

/* Hardware FIFO size: EZ_DATA_NR / 4 = (512 / 4) = 128 */
#define SCB_FIFO_SIZE	(128UL)

/*====== DIRECT PARAMETER for UARTx ======*/
#define UART_WORD_LEN_7		RX_CTRL_DATA_WIDTH(6)
#define UART_WORD_LEN_8		RX_CTRL_DATA_WIDTH(7)
#define UART_PARITY_NONE	UART_RX_CTRL_PARITY_DISABLE
#define UART_PARITY_EVEN	(UART_RX_CTRL_PARITY_ENABLE | UART_RX_CTRL_EVEN_PARITY)
#define UART_PARITY_ODD		(UART_RX_CTRL_PARITY_ENABLE | UART_RX_CTRL_ODD_PARITY)
#define UART_PARITY_MASK	(UART_RX_CTRL_PARITY_MASK)
#define UART_STOP_BIT_1		(UART_RX_CTRL_STOP_BIT(1))	/* 1 Stop Bit */
#define UART_STOP_BIT_1_5	(UART_RX_CTRL_STOP_BIT(2))	/* 1.5 Stop Bit */
#define UART_STOP_BIT_2		(UART_RX_CTRL_STOP_BIT(3))	/* 2 Stop Bit */
#define UART_STOP_BIT_MASK	UART_RX_CTRL_STOP_BIT_MASK
#define UART_HW_FLOW_DISABLE	(UART_FLOW_CTRL_CTS_DISABLE)
#define UART_HW_FLOW_CTS	(UART_FLOW_CTRL_CTS_ENABLE | UART_FLOW_CTRL_CTS_LOW | UART_FLOW_CTRL_TRIGGER_LEVEL_MASK)
#define UART_HW_FLOW_RTS	(UART_FLOW_CTRL_RTS_LOW | UART_FLOW_CTRL_TRIGGER_LEVEL_MASK)

#define UART_WORD_LEN	UART_WORD_LEN_8
#define UART_PARITY	UART_PARITY_NONE
#define UART_STOP_BIT	UART_STOP_BIT_1
#define UART_HW_FLOW	UART_HW_FLOW_DISABLE
#define UART_BAUD	(115200)	// = 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200

#define PCLK_SCB3_CLOCK	(33)

EXPORT void initUartParam( W port, UW wl, UW par, UW sb, UW baud, UW flow );

/*----------------------------------------------------------------------*/
/*	Register Base Address of UARTx					*/
/*	It is used to specify the register address with UART_XXX(x).	*/
/*----------------------------------------------------------------------*/
LOCAL	const	UW	uart_reg_base[MAX_UART_NO] = {
	UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE,
	UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE,
	UART8_BASE, UART9_BASE, UART10_BASE,
};

LOCAL	W	fifoNo;		// Number of FIFO entries
EXPORT	W	get_fifo_size( void )
{
	W fifo;
	if ((*SCBxUART_CTRL(TERM_PORT) & CTRL_WIDTH_MASK) == CTRL_WIDTH_BYTE)
	{
		fifo = SCB_FIFO_SIZE;
	}
	else
	{
		fifo = (SCB_FIFO_SIZE / 2U);
	}
	return fifo;
}

/*
 *	Set baurate clock
 *	clock_no: COMM root clocks
 *	divno: divider no
 *	div_val:  divider val(baurate)
 *
 *	UART baud rate = Input Clock [Hz] / OVS / divider val
 */
EXPORT	void	clock_set( UW clock_no, UW div_no, UW div_val )
{
	*PERI_PCLK_GR1_DIV_CMD |= DIV_CMD_DISABLE;
	*PERI_PCLK_GR1_DIV_CMD &= ~(DIV_SEL_MASK | TYPE_SEL_MASK);
	*PERI_PCLK_GR1_DIV_CMD |= (DIV_SEL(div_no) | TYPE_SEL_16);

	/* 16 divieder register selection, 1 divider */
	*PERI_PCLK_GR1_CLOCK_CTL(clock_no) = (DIV_SEL(div_no) | TYPE_SEL_16);

	/* Set the baud rate value in the divide-by-16 register */
	*PERI_PCLK_GR1_DIV_16_CTL(div_no) = DIV_16_CTRL_INT16_DIV(div_val);
	*PERI_PCLK_GR1_DIV_16_CTL(div_no) |= DIV_16_CTRL_EN;
	*PERI_PCLK_GR1_DIV_CMD |= DIV_CMD_ENABLE;
	while((*PERI_PCLK_GR1_DIV_16_CTL(div_no) & DIV_16_CTRL_EN) != DIV_16_CTRL_EN);
}

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while( (*SCBxTX_FIFO_STATUS(TERM_PORT) & SCB_TX_FIFO_STATUS_USED) >= fifoNo );	// Wait for free space in FIFO.
		*SCBxTX_FIFO_WR(TERM_PORT) = *b;
	}
	while( (*SCBxTX_FIFO_STATUS(TERM_PORT) & SCB_TX_FIFO_STATUS_USED) != 0 );	// Wait for Tx to complete.
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while( (*SCBxINTR_RX(TERM_PORT) & SCB_INTR_RX_NOT_EMPTY) == 0 );	// wait for Receive
		*buf = (UB)(*SCBxRX_FIFO_RD(TERM_PORT));
		if( (*SCBxRX_FIFO_STATUS(TERM_PORT) & SCB_RX_FIFO_STATUS_USED) == 0 ){	// RX FOFO Empty?
			*SCBxINTR_RX(TERM_PORT) = SCB_INTR_RX_NOT_EMPTY;		//  --> Clear Flag
		}
	}
}

EXPORT	void	tm_com_init(void)
{
	/* P13_0:SCB3_RX, P13_1:SCB3_TX */
	GPIO_PRT_CFG(GPIO_PRT13) &= 0xFFFFFF00;
	GPIO_PRT_CFG(GPIO_PRT13) |= ((CY_GPIO_DM_STRONG_IN_OFF << 4U) | (CY_GPIO_DM_HIGHZ << 0U));
	HSIOM_PRT_PORT_SEL0(HSIOM_PRT13) &= 0xFFFF0000;
	HSIOM_PRT_PORT_SEL0(HSIOM_PRT13) |= ((P13_0_SCB3_UART_RX << 0U) | (P13_1_SCB3_UART_TX << 8U));
	
	initUartParam( TERM_PORT, UART_WORD_LEN, UART_PARITY, UART_STOP_BIT, UART_BAUD, UART_HW_FLOW );
}

/*
 *	Initialize Uart Port.
 *	"wl", "par", "sb", "flow" are bitpatern (UART_XXX).
 *	"baud" must be over 1200(bps).
 */
EXPORT void initUartParam( W port, UW wl, UW par, UW sb, UW baud, UW flow )
{
	UW div_val = 0;

	/*------------------------------*/
	/*	Check Baudrate		*/
	/*------------------------------*/
	div_val = Cy_SysClk_ClkHfGetFrequency(2) / baud / 8;

	/*------------------------------*/
	/*	BAUDRATE		*/
	/*------------------------------*/
	/* Set to "16 bit Divider 0" to specify the baud rate. */
	clock_set(PCLK_SCB3_CLOCK, 0, div_val);

	/* Configure and enable the UART peripheral */
	*SCBxCTRL(port) = (CTRL_OVS(7) | CTRL_WIDTH_BYTE | CTRL_MODE_UART);
	*SCBxUART_CTRL(port) = UART_CTRL_MODE_STD;

	/*------------------------------*/
	/*	PARAMETERS		*/
	/*------------------------------*/
	/* Rx configurations */
	*SCBxUART_RX_CTRL(port) = (sb | par | UART_RX_CTRL_BREAK_WIDTH(10));
	*SCBxRX_CTRL(port) = (wl | RX_CTRL_LSB_FIRST);
	*SCBxRX_FIFO_CTRL(port) = RX_FIFO_CTRL_LEVEL(0);
	/* Tx configurations */
	*SCBxTX_CTRL(port) = (wl | TX_CTRL_LSB_FIRST);
	*SCBxUART_TX_CTRL(port) = (sb | par);

	/* interrupt configurations */
	*SCBxINTR_RX_MASK(port) = (SCB_INTR_RX_NOT_EMPTY);
	*SCBxINTR_TX_MASK(port) = (SCB_INTR_TX_EMPTY | SCB_INTR_TX_NOT_FULL);

	/* flow control configurations */
	*SCBxUART_FLOW_CTRL(port) = flow;

	/* Enable the UART peripheral */
	*SCBxCTRL(port) |= CTRL_ENABLED;

	fifoNo = get_fifo_size();
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* EVK_XMC7200 */
#endif /* USE_TMONITOR */
