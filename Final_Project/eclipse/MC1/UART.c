 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

/*                                 	ICU Header File                                                      */
#include "UART.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Function responsible to initialize UART module .
 */
void UART_init(void) {
	/* U2X = 1 for double transmission speed */
	SET_BIT(UCSRA, U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART TX Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	SET_BIT(UCSRB, TXEN);
	SET_BIT(UCSRB, RXEN);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	SET_BIT(UCSRC, URSEL);
	SET_BIT(UCSRC, UCSZ1);
	SET_BIT(UCSRC, UCSZ0);

	/*	select type of parity */
	SET_BIT(UCSRC, UPM1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = 12 >> 8;
	UBRRL = 12;

}

/*
 * Function responsible to send byte to another device .
 */
void UART_sendByte(const uint8 data) {

	/* UDRE flag is set when the TX buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/

}

/*
 * Function responsible to receive byte from another device .
 */
uint8 UART_recieveByte(void) {

	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the RX buffer (UDR) and the RXC flag
	   will be cleared after read this data */
    return UDR;
}

/*
 * Function responsible to send String from another device .
 */
void UART_sendString(const uint8 *Str) {

	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);		/* send string character by character until null character */
		i++;
	}
	UART_sendByte('#');
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/

}

/*
 * Function responsible to receive String from another device .
 */
void UART_receiveString(uint8 *Str) {

	uint8 i = 0;
	do{

		Str[i] = UART_recieveByte();

	}while(Str[i++] != '#');		/* still receive character until '#' symbol  send */

	Str[--i] = '\0';			/* add null character at the end of string */

}
