/******************************************************************************
 *
 * Module: TIMER - Configuration
 *
 * File Name: TIMER.h
 *
 * Description: File include all TIMER Configuration Declaration and Prototype .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_



/* My Header Files which will need in any Project */
#include "Macros.h"
#include "Micro_Config.h"
#include "Std_Types.h"

/* 								define Null Value														*/
#define NULL	((void *)0)

#define N								1024
#define TIME_SEC					(((F_CPU) / N) - 1)

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/*	choose run mode to Timer 1 */
typedef enum{
	OVF,CTC
}TIMER_Mode;

/*	choose PreScaler Frequency to Timer 1 */
typedef enum{
	F_CPU_1=1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}TIMER_Prescaler;

/*	Configuration Struct For Timer 1 */
typedef struct{
	TIMER_Mode mode;
	TIMER_Prescaler scale;
	uint16 ctc;
}TIMER_Config;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required clock.
 * 	2. Set the required Mode.
 * 	3. Enable the Interrupt for select Mode Timer1.
 * 	4. Initialize Timer1 Registers
 */
void TIMER_init(TIMER_Config *Str);

/*
 * Description: Function to Clear Timer1 Register & active Sleep mode For Timer1.
 */
void TIMER_Deinit(void);

/*
 * Description: Function to set Compare Value to Timer1 .
 */
void TIMER_SETCTC(uint16 *Ptr_CTC);

/*
 * Description: Function to set Which Function Called by Timer1  in ISR Vector.
 */
void TIMER_setCallBack(void (*Fun)(void));


#endif /* TIMER_H_ */
