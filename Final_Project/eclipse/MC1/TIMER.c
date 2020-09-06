/******************************************************************************
 *
 * Module: TIMER - Configuration
 *
 * File Name: TIMER.c
 *
 * Description: File include all TIMER Configuration Function .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/
/*                                 	TIMER Header File                                                      */
#include "TIMER.h"


/* Global Pointer to function use to select which function is Call Back . */
static volatile void (*g_CallBackPtr)(void) = NULL;

/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required clock.
 * 	2. Set the required Mode.
 * 	3. Enable the Interrupt for select Mode Timer1.
 * 	4. Initialize Timer1 Registers
 */
void TIMER_init(TIMER_Config *Str){
	TCCR1A=0;

	TCCR1B =(TCCR1B & 0xE7) | (Str->mode<<3);

	TCCR1B =(TCCR1B & 0xF8) | (Str->scale);

	TCNT1=0;

	if(Str->mode == CTC){
		OCR1A=(Str->ctc);
		TIMSK |=(1<<OCIE1A);
	}else if(Str->mode == OVF){
		TIMSK |=(1<<TOIE1);
	}
	SREG |=(1<<7);

}

/*
 * Description: Function to Clear Timer1 Register & active Sleep mode For Timer1.
 */
void TIMER_Deinit(void){
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	TIMSK=0;
	OCR1A=0;
}

/*
 * Description: Function to set Compare Value to Timer1 .
 */
void TIMER_SETCTC(uint16 *Ptr_CTC){
	TCNT1=0;
	OCR1A=*Ptr_CTC;
}

/*
 * Description: Function to set Which Function Called by Timer1  in ISR Vector.
 */
void TIMER_setCallBack(void (*Fun)(void)){
	g_CallBackPtr=Fun;
}

ISR(TIMER1_COMPA_vect){
	if(g_CallBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_CallBackPtr)();
	}

}

ISR(TIMER1_OVF_vect){
	if(g_CallBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_CallBackPtr)();
	}
}

