/******************************************************************************
 *
 * Module: MicroController 2  - Control_ECU - Configuration
 *
 * File Name: MC2.c
 *
 * Description: File include all MCU2 main task function prototype & variable declaration .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

/*					Modules Header Files						*/
#include "EEPROM.h"
#include "UART.h"
#include "TIMER.h"

#define ERROR					0
#define SUCCESS				1

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/* STATUS enum use to show state of garage door now ( open or close or ... etc ) */
typedef enum {
	OPENING, OPEN, CLOSING, CLOSE, BUZZER
} STATUS;

/* status global Variable use to show state of garage door now ( open or close or ... etc ) */
STATUS g_state = CLOSE;

/* Timer configuration structure use to initialize timer mode & preScaler & interrupt happen every 5 second */
TIMER_Config timePtr = { CTC, F_CPU_1024, TIME_SEC * 5 };

/* that variable use to receive byte from UART Module or ( read or write byte to EEPROM ) */
uint8 g_byte;

/* that variable use to Determines how many time of timer1 interrupt happen after initialize Timer .*/
uint8 timeIndex = 0;

/*		share password variable use to receive enter password from HMI-ECU	*/
uint64 g_pass = 0;

/*		share password variable use to check from enter password by HMI-ECU 	*/
uint64 g_passCheck = 0;

/* address of store location of password in EEPROM */
uint16 addPass = 0x0112;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Function to create password at first used or change password .
 */
void createNewPass(void);

/*
 * Description: Function use to check that password enter is equal to password or not to open garage .
 */
void changePass(void);

/*
 * Description: Function check that garage open first then close it .
 */
void closeDoor(void);

/*
 * Description: it is callBack Function that call by Timer when interrupt has happen .
 */
void TimerControl(void);

/*
 * Description: Function receive Password From HMI-ECU  &
 * pointer to select pass store variable .
 */
void enterPass(uint64 *data);

/*
 * Description: Function check password to open garage .
 */
void openDoor(void);

/*
 * Description: Function run Buzzer after user enter three time password wrong .
 */
void runBuzzer(void);

/*
 * Description: Function store password in EEPROM .
 */
void writePassInEEPROM(uint16 *add, uint64 *data);

/*
 * Description: Function read  stored password from  EEPROM .
 */
void readPassFromEEPROM(uint16 *add, uint64 *data);

#endif /* CONTROL_ECU_H_ */
