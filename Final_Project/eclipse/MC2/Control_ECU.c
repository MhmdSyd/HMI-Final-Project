/******************************************************************************
 *
 * Module: MicroController 2 -		Control_ECU 	- Configuration
 *
 * File Name: MC2.c
 *
 * Description: File include all MCU2 main task function definition & Main Function .
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

/*                                 	Control_ECU Header File                                                      */
#include "Control_ECU.h"

/*				main Control_ECU Function		*/
int main(void) {
	/* initialize EEPROM module  */
	EEPROM_init();
	UART_init(); /* initialize UART module  */

	/*		Determined function that call Back by Timer 	*/
	TIMER_setCallBack(TimerControl);

	/*		initial last three pins in PORTD (  Motor & Buzzer Pins )	*/
	DDRD |= 0xE0;
	PORTD &= 0x1F;

	/*  active this line when you want to reset the system */
	 //writePassInEEPROM(&addPass, &g_pass);


	/*  read password from EEPROM */
	readPassFromEEPROM(&addPass, &g_passCheck);
	_delay_ms(1000); /*	wait one second to synchronize with HMI-ECU */

	UART_sendByte('#'); /* tells MC1 that MC2 become ready & start communication with MC1 */

	/*  check that first time used or used before that time */
	if (g_passCheck == 0) {
		UART_sendByte(SUCCESS);/*  tells MC1 that time is first used for that module */
		createNewPass();/*  start to create password */
	} else {
		UART_sendByte(ERROR);/*  tells MC1 that module used before that time*/
	}
	/* Start of infinity loop */
	while (1) {
		g_byte = UART_recieveByte(); /*  wait until receive number of pressed key */

		/* check that pressed key is '+' key to start open door mode*/
		if (g_byte == '+') {
			openDoor();/*  start open door mode */
		} else if (g_byte == '-') { /* check that pressed key is '-' key to start change password mode*/
			changePass(); /*  start change password mode */
		}
		_delay_ms(500);
	}
	return 0;
}


/*
 * Description: Function to create password at first used or change password .
 */
void createNewPass(void) {
	do {
		enterPass(&g_pass); /* receive first password*/
		enterPass(&g_passCheck); /* receive second password*/
		if (g_pass == g_passCheck && g_pass != 0) { /* check that two password equal and pass not equal zero*/
			UART_sendByte(SUCCESS); /* tells MC1 that password is accepted & store in EEPROm*/
		} else {
			UART_sendByte(ERROR);/* tells MC1 that password is not accepted */
		}
		_delay_ms(1000); /*	wait one-half second to synchronize with HMI-ECU */
	} while (g_pass != g_passCheck); /* while password not accepted do that again*/
	writePassInEEPROM(&addPass, &g_pass); /* store password in EEPROM */
}

/*
 * Description: Function use to check that password enter is equal to password or not to open garage .
 */
void changePass(void) {

	enterPass(&g_pass); /* receive password from user*/
	readPassFromEEPROM(&addPass, &g_passCheck); /*  read password from EEPROM */
	if (g_pass == g_passCheck) { /* check that user enter correct password or not*/
		UART_sendByte(SUCCESS); /*  tells HMI_ECU that user enter correct password */
		createNewPass(); /*  start create new password */

	} else {
		UART_sendByte(ERROR); /*  tells HMI_ECU that user enter wrong password */
	}
}

/*
 * Description: Function check that garage open first then close it .
 */
void closeDoor(void) {
	while (UART_recieveByte() != '#'); /*  wait until MC1 is ready */

	while (UART_recieveByte() != '+') { /*  wait until user press in '+' key to start close garage */

		UART_sendByte(ERROR); /* tells MHI-ECU that user press on wrong key*/
	}
	/* tells MHI-ECU that user press on correct key and will start to close garage*/
	UART_sendByte(SUCCESS);
	/* initialize timer to start  count*/
	TIMER_init(&timePtr);

	/* edit output of motor pins to rotate motor anti-clockwise */
	SET_BIT(PORTD, PD5);
	CLEAR_BIT(PORTD, PD6);

	timeIndex = 0; /* Initialize timer interrupt counter */
	g_state = CLOSING; /* indicate that garage in closing mode */
	while (g_state == CLOSING); /*  wait until garage mode change to close mode */
}


/*
 * Description: it is callBack Function that call by Timer when interrupt has happen .
 */
void TimerControl(void) {
	/* Increment to show how many timer1 interrupt happen after initialize Timer  */
	timeIndex++;

	/* check first that garage state is closing to close it . */
	if (g_state == CLOSING) {

		/* check second that  time count equal ( 3 time * 5 second )15 second after timer initialize . */
		if (timeIndex >= 3) {

			/* Clear Timer1 Register & active Sleep mode For Timer1.*/
			TIMER_Deinit();

			/* Organize pins to stop motor rotate  */
			CLEAR_BIT(PORTD, PD5);
			CLEAR_BIT(PORTD, PD6);

			g_state = CLOSE;/*show state of garage door now is Close*/

			UART_sendByte(CLOSE); /*  Alerts HMI_ECU that  garage is close  */

			timeIndex = 0; /*  reset variable to start count from zero when timer initialize again */
		}
	} else if (g_state == BUZZER) { /* check first that garage state is in BUZZER mode */
		/* check second that time count equal ( 12 time * 5 second )60 second after timer initialize . */
		if (timeIndex >= 12) {
			UART_sendByte(CLOSE); /* tells HMI-ECU that garage is still close after buzzer stop*/
			TIMER_Deinit(); /* Clear Timer1 Register & active Sleep mode For Timer1.*/
			/* Organize pins to stop Buzzer */
			CLEAR_BIT(PORTD, PD7);
			g_state = CLOSE; /*show state of garage door now is Close*/

			timeIndex = 0; /*  reset variable to start count from zero when timer initialize again */
		}
	} else if (g_state == OPENING) { /* check first that garage state is in opening mode */

		/* check second that  time count equal ( 3 time * 5 second )15 second after timer initialize . */
		if (timeIndex >= 3) {
			TIMER_Deinit();

			UART_sendByte(OPEN); /* tells HMI-ECU that garage become open */
			/* Organize pins to stop motor rotate  */
			CLEAR_BIT(PORTD, PD5);/* Clear Timer1 Register & active Sleep mode For Timer1.*/
			CLEAR_BIT(PORTD, PD6);
			g_state = OPEN; /*show state of garage door now is open*/
			timeIndex = 0; /*  reset variable to start count from zero when timer initialize again */
		}
	}

}

/*
 * Description: Function receive Password From HMI-ECU  &
 * pointer to select pass store variable .
 */
void enterPass(uint64 *data) {
	(*data) = 0; /*  set variable that will receive password to zero */
	do {
		while (UART_recieveByte() != '*'); /*  wait until MC1 is ready */
		g_byte = UART_recieveByte(); /*  wait until receive byte from HMI_ECU  */

		if (g_byte <= 9) { /* check that key press is number or option key */
			(*data) = ((*data) * 10) + g_byte; /* store key in first digit in password & shift pass one digit to lift */
		}
		_delay_ms(400); /*  that wait to synchronize between human & ECU */
	} while (g_byte != '='); /* do that until press key is '=' */
}

/*
 * Description: Function run Buzzer after user enter three time password wrong .
 */
void runBuzzer(void) {
	/* run buzzer*/
	SET_BIT(PORTD, PD7);
	/* initialize timer to start count */
	TIMER_init(&timePtr);
	timeIndex = 0; /* Initialize timer interrupt counter */
	g_state = BUZZER; /* indicate that garage in Buzzer mode */
	while (g_state == BUZZER); /*  wait until garage mode change to close mode */

}

/*
 * Description: Function check password to open garage .
 */
void openDoor(void) {
	uint8 i = 0;
	do {
		enterPass(&g_pass); /*  receive password from HMI-ECU */
		/*  read password from EEPROM */
		readPassFromEEPROM(&addPass, &g_passCheck);

		UART_sendByte('#'); /*  tells HMI-ECU that MC2 become ready */

		if (g_pass == g_passCheck) { /* check that user enter correct password or not */
			UART_sendByte(ERROR); /*  tells HMI_ECU that user enter correct password */
			_delay_ms(10);
			UART_sendByte(SUCCESS);
			/* initialize timer to start  count*/
			TIMER_init(&timePtr);
			/* edit output of motor pins to rotate motor clockwise */
			CLEAR_BIT(PORTD, PD5);
			SET_BIT(PORTD, PD6);

			timeIndex = 0; /* Initialize timer interrupt counter */
			g_state = OPENING; /* indicate that garage in opening mode */
			while (g_state == OPENING); /*  wait until garage mode change to open mode */
			closeDoor(); /*  start with close mode */
			i = 10; /* choose (10) that any number > 3 to break while loop*/
		} else {
			UART_sendByte(ERROR); /*  tells HMI_ECU that user enter wrong password */
			_delay_ms(10);
			UART_sendByte(ERROR);
			/* wait one second to synchronize with HMI-ECU that now display error massage */
			_delay_ms(1000);
			i++;
		}
	} while (i < 3); /* wait until user enter correct pass or three time wrong password*/
	if (i == 3) { /* that means user enter three time wrong password*/
		runBuzzer(); /* start of buzzer mode */
	}

}

/*
 * Description: Function store password in EEPROM .
 */
void writePassInEEPROM(uint16 *add, uint64 *data) {
	uint8 i;
	for (i = 0; i < 8; i++) {
		/* write i-byte of password in select location + i*/
		EEPROM_writeByte(((*add) + i), ((*data) >> (8 * i)));
		_delay_ms(10); /* wait to write byte in EEPROM correctly */
	}
}

/*
 * Description: Function read  stored password from  EEPROM .
 */
void readPassFromEEPROM(uint16 *add, uint64 *data) {
	uint8 i;
	for (i = 0; i < 8; i++) {
		/* read i-byte of password in select location + i */
		EEPROM_readByte(((*add) + i), &g_byte);
		_delay_ms(10); /* wait to read byte in EEPROM correctly */
		/* store read i-byte in i-byte of password variable*/
		(*data) = (*data) | ((uint32) g_byte << (8 * i));
	}
}
