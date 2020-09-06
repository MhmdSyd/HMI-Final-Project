/******************************************************************************
 *
 * Module: MicroController 1 - Configuration
 *
 * File Name: MC1.c
 *
 * Description: File include all MicroController1 main task
 *
 * Author: Mohamed Elsayed
 *
 *******************************************************************************/

/*                                 	HMI Header File                                                      */
#include "HMI_ECU.h"

/*				main HMI_ECU Function		*/
int main(void) {
	UART_init(); /* initialize UART module*/
	LCD_init(); /* initialize LCD module*/

	/*		Determined function that call Back by Timer 	*/
	TIMER_setCallBack(TimerControl);
	/*print start massage on LCD screen*/
	LCD_displayStringRowColumn(0, 3, "Welcome !");
	_delay_ms(1000);/* wait one second to show massage*/

	/* Wait until HMI_ECU is ready */
	while (UART_recieveByte() != '#');
	/*wait until MC2 Determine that is first used or not and receive Reply */
	if (UART_recieveByte() == SUCCESS) {
		createNewPass(); /*  start to create password */
	} else {
	}
	/* Start of infinity loop */
	while (1) {
		mainOption(); /* print option on LCD Screen choose open door or change password*/
		/* receive press key from KeyPad */
		key = KeyPad_getPressedKey();
		if (key == '+') {
			openDoor(); /*if user enter '+' key start to open door mode*/
		} else if (key == '-') {
			changePass(); /*if user enter ''-' key start to change pass mode*/
		}
		_delay_ms(500);
	}
	return 0;
}

/*
 * Description: Function print option on LCD Screen ( open door or change pass ) .
 */
void mainOption(void) {
	/* print option on LCD Screen ( open door or change pass )*/
	LCD_clearScreen();
	LCD_displayString(" + : open Door ");
	LCD_goToRowColumn(1, 0);
	LCD_displayString(" - : Change Pass ");
}

/*
 * Description: Function to create password at first used or change password .
 */
void createNewPass(void) {
	uint8 stat;
	do {
		/* enter first pass*/
		enterPass(" Enter New Pass ");
		enterPass(" Re-enter Pass ");		/* enter second pass*/
		/* wait until MC2 determine that two pass is equal or not*/
		if (UART_recieveByte() == SUCCESS) {
			LCD_clearScreen();
			/* tells user that enter pass is accepted & store in EEPROM */
			LCD_displayString("Pass is Created");
			stat = SUCCESS;	/* make stat success to break while loop*/
		} else {
			LCD_clearScreen();
			/* tells user that enter pass is not accepted */
			LCD_displayString(" ERROR Password ");
			stat = ERROR;/* make stat success to do this again while loop*/
		}
		_delay_ms(1000);
	} while (stat != SUCCESS); /*do this until user enter two equal password*/
}

/*
 * Description: Function use to check that password enter is equal to password or not to open garage .
 */
void changePass(void) {
	/*enter password by user */
	enterPass(" Enter old Pass");
	/*take replay from MC2 that entered pass is correct or not*/
	if (UART_recieveByte() == SUCCESS) {
		createNewPass(); /*start create new pass*/
	} else {
		LCD_clearScreen();
		/*print error massage that means user enter incorrect pass*/
		LCD_displayString("  ERROR Pass ");
	}
}

/*
 * Description: Function check that garage open first then close it .
 */
void closeDoor(void) {
	/* tells MC2 that MC1 become ready & start communication */
	UART_sendByte('#');
	do {
		/*send press key from Key pad to MC2*/
		UART_sendByte(KeyPad_getPressedKey());
		/*receive replay from MC2 to know that is correct key is press or not*/
	} while (UART_recieveByte() != SUCCESS);

	LCD_clearScreen();
	/*tells user that garage start closing by print massage on LCD */
	LCD_displayString(" Door is Locking ");
	TIMER_init(&timePtr);	/* initialize timer to start  count*/
	timeIndex = 0;	/* Initialize timer interrupt counter */
	g_state = CLOSING;	/* indicate that garage in closing mode */
	while (g_state == CLOSING);	/*  wait until garage mode change to close mode */

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
			/*receive state of garage door */
			if (UART_recieveByte() == CLOSE) {
				g_state = CLOSE; /*change global state of garage door to close state*/
				LCD_clearScreen();
				/*print closed massage that indicate of garage door state */
				LCD_displayString(" Door is Closed ");
			}
		}

	} else if (g_state == BUZZER) { /* check first that garage state is in BUZZER mode */
		/* check second that time count equal ( 12 time * 5 second )60 second after timer initialize . */
		if (timeIndex >= 12) {
			/* take replay from MC2 that garage door is close*/
			if (UART_recieveByte() == CLOSE) {
				g_state = CLOSE; /*show state of garage door now is Close*/
				/* Clear Timer1 Register & active Sleep mode For Timer1.*/
				TIMER_Deinit();
				LCD_clearScreen();
				/*print closed massage that indicate of garage door state */
				LCD_displayString(" Door is Closed ");
			}
		}
	} else if (g_state == OPENING) { /* check first that garage state is in opening mode */

		/* check second that  time count equal ( 3 time * 5 second )15 second after timer initialize . */
		if (timeIndex >= 3) {
			/* Clear Timer1 Register & active Sleep mode For Timer1.*/
			TIMER_Deinit();
			/* take replay from MC2 that garage door is open*/
			if (UART_recieveByte() == OPEN) {
				g_state = OPEN; /*show state of garage door now is open*/
				LCD_clearScreen();
				/*print open massage that indicate of garage door state */
				LCD_displayString(" Door is Opened ");
				LCD_goToRowColumn(1, 0);
				/*print massage that indicate to key to close garage door */
				LCD_displayString("+: to Close Door");
			}
		}

	}

}

/*
 * Description: Function receive press key  From KeyPad & send to Control -ECU
 * pointer to select which massage will appear in LCD Screen .
 */
void enterPass(const char *Str) {
	LCD_clearScreen();
	/*print determined massage*/
	LCD_displayString(Str);
	LCD_goToRowColumn(1, 0);
	do {
		/* tells MC2 that MC1 become ready & start communication */
		UART_sendByte('*');
		/*receive press key from KeyPad Module*/
		key = KeyPad_getPressedKey();
		/*check if press key is number or option key*/
		if ((key >= 48) && (key <= 57)) {
			key -= 48; /*minus 48 that is ASCII of zero number*/
			UART_sendByte(key); /*send number of press key */
			LCD_displayCharacter('*'); /*display '*' to indicate that key is send to MC2 */
		} else {
			/*if press key is option key send by ASCII*/
			UART_sendByte(key);
		}
		_delay_ms(400);/*  that wait to synchronize between human & ECU */
	} while (key != '=');
}

void runBuzzer(void){

	LCD_clearScreen();
	/* print thief massage */
	LCD_displayString(" You are Thief!");
	/* initialize timer to start count */
	TIMER_init(&timePtr);
	timeIndex=0;	 /* Initialize timer interrupt counter */
	g_state=BUZZER;	 /* indicate that garage in Buzzer mode */
	while(g_state == BUZZER);/*  wait until garage mode change from Buzzer mode */

}

/*
 * Description: Function check enter password by user to open garage .
 */
void openDoor(void){
	uint8 i=0;
	do {
		/*recive password from KeyPad & send to MC2 */
		enterPass(" Plz enter Pass");
		/*  wait until MC2 is ready */
		while(UART_recieveByte() == '#');
		/* check that entered  pass is correct or not*/
		if (UART_recieveByte() == SUCCESS) {
			LCD_clearScreen();
			/* print massage that indicate garage door is start open */
			LCD_displayString("Door is Opening");
			/* initialize timer to start  count*/
			TIMER_init(&timePtr);
			timeIndex = 0;	/* Initialize timer interrupt counter */
			g_state = OPENING;	/* indicate that garage in opening mode */
			while(g_state == OPENING);	/*  wait until garage mode change to open mode */
			closeDoor();	/*  start with close mode */
			i=10;	/* choose (10) that any number > 3 to break while loop*/
		} else {
			LCD_clearScreen();
			/* print massage that entered pass is incorrect */
			LCD_displayString(" 	 ERROR Pass ");
			_delay_ms(1000);
			i++;
		}
	} while (i < 3);	/* wait until user enter correct pass or three time wrong password*/
	if(i == 3){	/* that means user enter three time wrong password*/
		runBuzzer();	 /* start of buzzer mode */
	}

}
