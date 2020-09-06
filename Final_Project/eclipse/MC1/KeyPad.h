/******************************************************************************
 *
 * Module: KeyPad - Configuration
 *
 * File Name: KeyPad.h
 *
 * Description: File include all KeyPad Configuration Declaration and Prototype .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/* Configuration Number of Row and Column */
#define N_col  4
#define N_row 4

/* Configuration Which PORT  attach with KeyPad */
#define KEYPAD_PORT PORTA
#define KEYPAD_PIN    PINA
#define KEYPAD_DIR    DDRA

/* My Header Files which will need in any Project */
#include "Macros.h"
#include "Micro_Config.h"
#include "Std_Types.h"

/*******************************************************************************
 *                     KeyPad Functions deceleration                                   *
 *******************************************************************************/

/*
 * Function responsible for getting the pressed keypad key
 */

uint8 KeyPad_getPressedKey(void);



#endif /* KEYPAD_H_ */
