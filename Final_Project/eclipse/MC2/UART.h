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

#ifndef UART_H_
#define UART_H_

#include "Macros.h"
#include "Std_Types.h"
#include "Micro_Config.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* UART Driver Baud Rate */
#define USART_BAUDRATE 9600

#define DOUBLE_SPEED			TRUE

#define UCSRA_REG				(*(volatile REG * const )0x2B)
#define UCSRB_REG				(*(volatile REG * const )0x2B)
#define UCSRC_REG				(*(volatile REG * const )0x2B)

#define D2X							UCSRA_REG.BITS.BIT1

#define UBBR							(((F_CPU) / (USART_BAUDRATE * 8 )) -1)

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Function responsible to initialize UART module .
 */
void UART_init(void);

/*
 * Function responsible to send byte to another device .
 */
void UART_sendByte(const uint8 data);

/*
 * Function responsible to receive byte from another device .
 */
uint8 UART_recieveByte(void);

/*
 * Function responsible to send String from another device .
 */
void UART_sendString(const uint8 *Str);

/*
 * Function responsible to receive String from another device .
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
