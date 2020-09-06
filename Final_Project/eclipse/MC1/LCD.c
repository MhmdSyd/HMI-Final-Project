/******************************************************************************
 *
 * Module: LCD - Configuration
 *
 * File Name: LCD.c
 *
 * Description: File include all LCD Configuration Function .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/
/*                                 	LCD Header File                                                      */

#include "LCD.h"

/*******************************************************************************
 *                    			   LCD Functions definition                                   *
 *******************************************************************************/

/*
 * Function responsible for Send Order to LCD
 */
void LCD_sendCommand(uint8 command) {
	RS = CLEAR; /* tell LCD that data which will send now is command */
	RW = CLEAR; /* select write Mode on LCD */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	E = SET; /* open Enable to send data from AVR to LCD */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
#if(DATA_MODE ==8)
	DATA_OUT = command; /* send command to LCD to make order */
#endif

#if(DATA_MODE ==4)
#ifdef UPPER_MODE_BITS
	/* send the highest 4 bits of the required command to the data bus D4 --> D7 */
	DATA_OUT=( command & 0xF0 ) | ( DATA_OUT & 0x0F );
#endif
#ifdef LOWER_MODE_BITS
	DATA_OUT = ((command >> 4) & 0x0F) | ( DATA_OUT & 0xF0);
#endif

	E = CLEAR; /* close Enable to prevent data share between LCD and AVR */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	E = SET; /* open Enable again to send last 4 bits of command */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

#ifdef UPPER_MODE_BITS
	/* send the Least 4 bits of the required command to the data bus D0 --> D3 */
	DATA_OUT=( (command << 4 ) & 0xF0 ) | ( DATA_OUT & 0x0F );
#endif

#ifdef LOWER_MODE_BITS
	DATA_OUT = ((command) & 0x0F) | ( DATA_OUT & 0xF0);
#endif
#endif
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	E = CLEAR; /* close enable to prevent data share between LCD and AVR */
	_delay_ms(1); /* delay for processing Th = 13ns */
	return;
}

/*
 * Function responsible for Display only One character on LCD
 */
void LCD_displayCharacter(uint8 data) {
	RS = SET; /* tell LCD that data which will send now is DATA */
	RW = CLEAR; /* select write Mode on LCD */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	E = SET; /* open Enable to send data from AVR to LCD */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
#if(DATA_MODE ==8)
	DATA_OUT = data; /* send DATA to LCD to make order */
#endif

#if(DATA_MODE ==4)
#ifdef UPPER_MODE_BITS
	/* send the highest 4 bits of the required DATA to the data bus D4 --> D7 */
	DATA_OUT=( data & 0xF0 ) | ( DATA_OUT & 0x0F );
#endif
#ifdef LOWER_MODE_BITS
	DATA_OUT = ((data >> 4) & 0x0F) | ( DATA_OUT & 0xF0);
#endif

	E = CLEAR; /* close Enable to prevent data share between LCD and AVR */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	E = SET; /* open Enable again to send last 4 bits of DATA */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

#ifdef UPPER_MODE_BITS
	/* send the Least 4 bits of the required DATA to the data bus D4 --> D7 */
	DATA_OUT=( (data << 4 ) & 0xF0 ) | ( DATA_OUT & 0x0F );
#endif

#ifdef LOWER_MODE_BITS
	DATA_OUT = ((data) & 0x0F) | ( DATA_OUT & 0xF0);
#endif
#endif
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	E = CLEAR; /* close enable to prevent data share between LCD and AVR */
	_delay_ms(1); /* delay for processing Th = 13ns */
	return;
}

/*
 * Function responsible for Display more One character on LCD
 */
void LCD_displayString(const char *Str) {
	/***************** Another Method ***********************
	while (*Str != '\0') {
		LCD_displayCharacter(*Str);
		Str++;
	}
	************************************************************/

	 uint8 i = 0;
	 while(Str[i] != '\0')
	 {
	 LCD_displayCharacter(Str[i]);
	 i++;
	 }
	return;

}

/*
 * Function responsible to initialize LCD pins and mode .... etc .
 */
void LCD_init(void) {
	RS_DIR = SET; /* select RS pin output pin */
	RW_DIR = SET; /* select RW pin output pin */
	E_DIR = SET; /* select Enable pin output pin */
#if (DATA_MODE == 8)
	DATA_DIR = 0xFF; /* select LCD DATA PORT Output pin */
	/* LCD Configuration Mode of number of DATA line = 8 lines */
	LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE);
#endif

#if (DATA_MODE == 4)
	/* LCD Configuration Mode of number of DATA line = 4 lines */
	LCD_sendCommand(FOUR_BITS_MODE); /* initialize LCD in 4-bit mode */
	LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE);
#ifdef UPPER_MODE_BITS
	DATA_DIR |= 0xF0; /* select highest 4 bits as a output pins */
#endif

#ifdef LOWER_MODE_BITS
	DATA_DIR |= 0x0F; /* select first 4 bits as a output pins */
#endif
#endif
	LCD_sendCommand(CURSOR_OFF); /* hide cursor from LCD */
	LCD_sendCommand(AUTO_INCREMENT); /* hide cursor from LCD */
	LCD_sendCommand(CLEAR_SCREEN); /* clear LCD screen */
	_delay_ms(2);
	LCD_sendCommand(CURSOR_HOME_LOCATION);
	return;
}

/*
 * Function responsible to send clear command to LCD
 */
void LCD_clearScreen(void) {
	LCD_sendCommand(CLEAR_SCREEN); /* clear LCD screen */
	LCD_sendCommand(CURSOR_HOME_LOCATION);
	return;
}

/*
 * Function responsible for Display more One character on LCD & select place on screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str) {
	/* call LCD_goToRowColumn function to select cursor location */
	LCD_goToRowColumn(row, col);
	/* call LCD_displayString function to display string on LCD */
	LCD_displayString(Str);
	return;
}

/*
 * Function responsible to select place on screen
 */
void LCD_goToRowColumn(uint8 row, uint8 col) {
	uint8 address;
	switch (row) {
	case 0:
		address = 0x80 | col; /* first address in first line in LCD = 0x00 */
		break;
	case 1:
		address = col | 0xC0; /* first address in second line in LCD = 0x40 */
		break;
	case 2:
		address = col | 0x10; /* first address in 3th line in LCD = 0x10 */
		break;
	case 3:
		address = col | 0x50; /* first address in 4th line in LCD = 0x50 */
		break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0x80+Address */
	LCD_sendCommand(address | CURSOR_HOME_LOCATION);
}

/*
 * Function that transform number to string and display to LCD
 */
void LCD_intgerToString(int data) {
	char buff[16]; /* String to hold the ASCII result */
	itoa(data, buff, 10); /* 10 for decimal */
	LCD_displayString(buff);
}

/*
 * Function that Create Custom Character in CGRAM
 */
void LCD_creatCustomChar(uint8 loc, uint8 *msg) {
	uint8 i;
	if (loc < 8) {
		LCD_sendCommand(0x40 | (loc * 8)); /* Command 0x40 and onwards forces
		 the device to point CGRAM address */
		for (i = 0; i < 8; i++) { /* Write 8 byte for generation of 1 character */
			LCD_displayCustomChar(msg[i]);
		}
	}
}

/*
 * Function that Display Custom Character in CGRAM
 */
void LCD_displayCustomChar(uint8 char_data) /* LCD data write function */
{

#if(DATA_MODE ==8)
	DATA_OUT = char_data; /* send DATA to LCD to make order */
#endif

#if(DATA_MODE ==4)
#ifdef UPPER_MODE_BITS
	/* send the highest 4 bits of the required DATA to the data bus D4 --> D7 */
	DATA_OUT=( char_data & 0xF0 ) | ( DATA_OUT & 0x0F );
#endif
#ifdef LOWER_MODE_BITS
	DATA_OUT = ((char_data >> 4) & 0x0F) | ( DATA_OUT & 0xF0);
#endif

	RS = SET;/* RS=1 Data reg. */
	RW = CLEAR; /* RW=0 write operation */
	E = SET; /* Enable Pulse */
	_delay_us(1);
	E = CLEAR;
	_delay_ms(2); /* Data write delay */
#ifdef UPPER_MODE_BITS
	/* send the Least 4 bits of the required DATA to the data bus D4 --> D7 */
	DATA_OUT=( (char_data << 4 ) & 0xF0 ) | ( DATA_OUT & 0x0F );
#endif

#ifdef LOWER_MODE_BITS
	DATA_OUT = ((char_data) & 0x0F) | ( DATA_OUT & 0xF0);
#endif
#endif
	RS = SET;/* RS=1 Data reg. */
	RW = CLEAR; /* RW=0 write operation */
	E = SET; /* Enable Pulse */
	_delay_us(1);
	E = CLEAR;
	_delay_ms(2); /* Data write delay */
	return;

}
