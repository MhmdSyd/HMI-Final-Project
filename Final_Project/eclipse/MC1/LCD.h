/******************************************************************************
 *
 * Module: LCD - Configuration
 *
 * File Name: LCD.h
 *
 * Description: File include all LCD Configuration Declaration and Prototype .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/
#ifndef LCD_H_
#define LCD_H_

/* My Header Files which will need in any Project */
#include "Macros.h"
#include "Micro_Config.h"
#include "Std_Types.h"

/* LCD Configuration Data Send 8 bits or 4 bits */
#define DATA_MODE		8

#if(DATA_MODE ==4)

/* Choose that first or last 4 bit will send data to LCD  UPPER or LOWER */
#define LOWER_MODE_BITS

#endif
/* RS PIN Configuration control that send data or command */
#define RS_DIR				DDRD_REG.BITS.BIT4
#define RS 						PORTD_REG.BITS.BIT4

/* RW PIN that control Read or Write from LCD */
#define RW_DIR				DDRD_REG.BITS.BIT5
#define RW						PORTD_REG.BITS.BIT5

/* E means enable that control enable data share between LCD and AVR or not */
#define E_DIR					DDRD_REG.BITS.BIT6
#define E						PORTD_REG.BITS.BIT6

/* DATA LCD PORT which connect with AVR */
#define DATA_DIR 			DDRC
#define DATA_OUT			PORTC
#define DATA_PIN			PINC

/* LCD Command numbers define */
#define CLEAR_SCREEN 										0x01
#define FOUR_BITS_MODE 								0x02
#define TWO_LINE_LCD_Four_BIT_MODE 			0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 			0x38
#define CURSOR_OFF											0x0C
#define CURSOR_ON 											0x0E
#define CURSOR_HOME_LOCATION 					0x80
#define SHIFT_DISPLAY_LEFT								0x28
#define SHIFT_DISPLAY_RIGHT							0x1C
#define MOVE_CURSOR_LEFT								0x10
#define MOVE_CURSOR_RIGHT							0x14
#define CURSOR_BLINKING								0x0F
#define AUTO_INCREMENT								0x06

/*******************************************************************************
 *                     LCD Functions deceleration                                   *
 *******************************************************************************/

/*
 * Function responsible for Send Order to LCD
 */
void LCD_sendCommand(uint8 command);

/*
 * Function responsible for Display only One character on LCD
 */
void LCD_displayCharacter(uint8 data);

/*
 * Function responsible for Display more One character on LCD
 */
void LCD_displayString(const char *Str);

/*
 * Function responsible to initialize LCD pins and mode .... etc .
 */
void LCD_init(void);

/*
 * Function responsible to send clear command to LCD
 */
void LCD_clearScreen(void);

/*
 * Function responsible for Display more One character on LCD & select place on screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);

/*
* Function responsible to select place on screen
*/
void LCD_goToRowColumn(uint8 row,uint8 col);

/*
* Function that transform number to string and display to LCD
*/
void LCD_intgerToString(int data);

/*
* Function that Create Custom Character in CGRAM
*/
void LCD_creatCustomChar (uint8 loc, uint8 *msg);

/*
* Function that Display Custom Character in CGRAM
*/
void LCD_displayCustomChar (uint8 char_data);

#endif /* LCD_H_ */
