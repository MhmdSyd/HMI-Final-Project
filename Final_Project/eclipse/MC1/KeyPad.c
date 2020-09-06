/******************************************************************************
 *
 * Module: KeyPad - Configuration
 *
 * File Name: KeyPad.c
 *
 * Description: File include all KeyPad Configuration Function .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

#include "KeyPad.h"

/*
 * Function responsible for getting ASCII code for the pressed keypad key
 */
#if(N_col ==3)
static uint8 KeyPad_4x3_ReturnASCII(uint8 num );

#elif(N_col ==4)

static uint8 KeyPad_4x4_ReturnASCII(uint8 );

#endif



/*
 * Function responsible for getting the pressed keypad key
 */
uint8 KeyPad_getPressedKey(void) {

	uint8 col,row;
		while(1)
		{
			for(col=0;col<N_col;col++) /* loop for columns */
			{
				/*
				 * each time only one of the column pins will be output and
				 * the rest will be input pins include the row pins
				 */
				KEYPAD_DIR = (0b00010000<<col);

				/*
				 * clear the output pin column in this trace and enable the internal
				 * pull up resistors for the rows pins
				 */
				KEYPAD_PORT = (~(0b00010000<<col));
				for(row=0;row<N_row;row++) /* loop for rows */
				{
					if(BIT_IS_CLEAR(KEYPAD_PIN,row)) /* if the switch is press in this row */
					{
						#if (N_col == 3)
							return KeyPad_4x3_ReturnASCII((row*N_col)+col+1);
						#elif (N_col == 4)
							return KeyPad_4x4_ReturnASCII((row*N_col)+col+1);
						#endif
					}
				}
			}
		}
	}

#if(N_col ==3)
static uint8 KeyPad_4x3_ReturnASCII(uint8 num) {
	switch (num) {
	case 1:		return '1';
		break;
	case 2:		return '2';
		break;
	case 3:		return '3';
		break;
	case 4:		return '4';
		break;
	case 5:		return '5';
		break;
	case 6:		return '6';
		break;
	case 7:		return '7';
		break;
	case 8:		return '8';
		break;
	case 9:		return '9';
		break;
	case 10:	return '*';
		break;
	case 11:	return '0';
		break;
	case 12:	return '#';
		break;
	default:	return '\0';
		break;
	}
	return 0;
}
#elif(N_col ==4)
static uint8 KeyPad_4x4_ReturnASCII(uint8 num) {
	switch (num) {
	case 1:		return '7';
		break;
	case 2:		return '8';
		break;
	case 3:		return '9';
		break;
	case 4:		return '/';
		break;
	case 5:		return '4';
		break;
	case 6:		return '5';
		break;
	case 7:		return '6';
		break;
	case 8:		return '*';
		break;
	case 9:		return '1';
		break;
	case 10:	return '2';
		break;
	case 11:	return '3';
		break;
	case 12:	return '-';
		break;
	case 13:	return 8;
		break;
	case 14:	return '0';
		break;
	case 15:	return '=';
		break;
	case 16:	return '+';
			break;
	default:	return '\0';
		break;
	}
	return 0;
}
#endif
