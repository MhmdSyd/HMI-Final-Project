/******************************************************************************
 *
 * Module: MicroController 1  - Control_ECU - Configuration
 *
 * File Name: MC1.c
 *
 * Description: File include all MCU1 main task function prototype & variable declaration .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

/*					Modules Header Files						*/
#include "LCD.h"
#include "UART.h"
#include "TIMER.h"

#define ERROR					0
#define SUCCESS				1

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/* STATUS enum use to show state of garage door now ( open or close or ... etc ) */
typedef enum{
	OPENING,OPEN,CLOSING,CLOSE,BUZZER
}STATUS;

/* Timer configuration structure use to initialize timer mode & preScaler ... etc */
TIMER_Config timePtr ={CTC,F_CPU_1024,TIME_SEC * 5};

/* status Variable use to show state of garage door now ( open or close or ... etc ) */
STATUS g_state=CLOSE;

/* that variable use to receive pressed key from KeyPad Module */
uint8 key ;

/* that variable use to Determines how many timer1 interrupt happen after initialize Timer .*/
uint8 timeIndex=0;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Function to create password at first used or change password .
 */
void createNewPass(void);

/*
 * Description: Function print option on LCD Screen ( open door or change pass ).
 */
void mainOption(void);

/*
 * Description: Function check enter password by user to open garage .
 */
void openDoor(void);

/*
 * Description: Function receive press key  From KeyPad & send to Control -ECU
 * pointer to select which massage will appear in LCD Screen .
 */
void enterPass(const char *Str);

/*
 * Description: Function check that garage open first then close it .
 */
void closeDoor(void);

/*
 * Description: Function run Buzzer after user enter three time password wrong .
 */
void runBuzzer(void);

/*
 * Description: Function use to check that password enter is equal to password or not to open garage .
 */
void changePass(void);

/*
 * Description: it is callBack Function that call by Timer when interrupt has happen .
 */
void TimerControl(void);




#endif /* HMI_ECU_H_ */
