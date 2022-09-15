/*
 * MC1main.c
 *
 *  Created on: Oct 24, 2021
 *      Author: Ali Hany
 */

#include "MC1main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "keypad.h"
#include "lcd.h"
#include "Timer.h"
#include "uart.h"




int main(void)
{

	SREG |= (1 << 7); // enable global interrupt //

	LCD_init();

	Uart_Config Uart = {_8_BIT,DISABLED,_1_BIT};

	UART_init(9600,&Uart);

	/* Timer freq = 8MHz/1024, one clock-cycle time = 128 uSecond
			 so to force the timer to produce an interrupt every 1 second:
			 we set the compare value to be 1/(128u) = 7813 */

	TIMER_Config timer_config = {Timer1,CTC,F_CPU_1024,7813,0};
	Timer_init(&timer_config);
	Timer1_setCallBack(timerCallBack);

	initializePassword(); /* initialize first-time password */
	appMainOptions(); /* Display application options */
	uint8 receivedByte = 0, key = 0;

	while(1)
	{
		key = KEYPAD_getPressedKey();
		if (key == '+')
		{
			LCD_clearScreen();
			LCD_displayString("Enter the Pass:");
			getPassword(g_inputPassword);
			UART_sendByte(READY_TO_SEND); /* inform Control ECU to start sending */
			sendPasswordViaUART(g_inputPassword);
			/* inform Control ECU the option that user chose */
			UART_sendByte('+');

			/* Control ECU responses [either the password is correct or wrong] */
			receivedByte = UART_recieveByte();
			if (receivedByte == UNLOCKING_DOOR)
			{
				DoorOpeningTask(); /* start displaying door status on LCD */

			}
			else if (receivedByte == WRONG_PASSWORD)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(DISPLAY_MESSAGE_DELAY);
			}
			appMainOptions(); /* system back to idle & display main options */


		}
		else if (key == '-')
		{
			LCD_clearScreen();
			LCD_displayString("Enter Old Pass:");
			getPassword(g_inputPassword);
			UART_sendByte(READY_TO_SEND); /* inform Control ECU to start sending */
			sendPasswordViaUART(g_inputPassword);

			/* inform Control ECU the option that user chose */
			UART_sendByte(CHANGE_PASSWORD_OPTION);

			receivedByte = UART_recieveByte();
			if (receivedByte == CHANGING_PASSWORD)
			{
				initializePassword();
				LCD_clearScreen();
			}
			else if (receivedByte == WRONG_PASSWORD)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(DISPLAY_MESSAGE_DELAY);
			}
			appMainOptions();
		}
	}
}



/********************************************************/
/*             Functions Defintions                     */
/********************************************************/

void getPassword(uint8 * arrayName)
{
	LCD_moveCursor(1, 0);
	uint8 i = 0;
	uint8 key;

	while(i != PASSWORD_LENGTH)
	{
		key = KEYPAD_getPressedKey();
		if (key >= 0 && key <= 9)
		{
			LCD_displayCharacter('*');
			*(arrayName + i) = key;
			i++;
		}
		_delay_ms(KEYPAD_INPUT_DELAY);
	}
	key = 0;

	/* stay till user pushes Enter */
	while(KEYPAD_getPressedKey() != 13);
}

void appMainOptions(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+: Open Door");
	LCD_displayStringRowColumn(1, 0, "-: Change Pass");
}

void initializePassword(void)
{
	while(g_password_match_status == PASSWORD_MISMATCHED)
	{
		LCD_clearScreen();
		LCD_displayString("Enter Pass:");
		LCD_moveCursor(1, 0);
		getPassword(g_inputPassword); /* get the password from user */
		UART_sendByte(READY_TO_SEND);
		while (UART_recieveByte() != READY_TO_RECEIVE); /* wait for a response */
		sendPasswordViaUART(g_inputPassword);

		/* get confirm password from user */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Re-enter Pass:");
		LCD_moveCursor(1, 0);
		getPassword(g_inputPassword);
		UART_sendByte(READY_TO_SEND);
		while (UART_recieveByte() != READY_TO_RECEIVE);
		sendPasswordViaUART(g_inputPassword);

		/* wait for a response from Control ECU about passwords matching */
		while (UART_recieveByte() != READY_TO_SEND);
		g_password_match_status = UART_recieveByte();

		if (g_password_match_status == PASSWORD_MISMATCHED)
		{
			LCD_clearScreen();
			LCD_displayString("Pass Mismatch");
			_delay_ms(DISPLAY_MESSAGE_DELAY);
		}
	}
	g_password_match_status = PASSWORD_MISMATCHED; /* reset password match to MISMATCH for later usage */
}

void timerCallBack(void)
{
	g_seconds++;
}

void DoorOpeningTask(void)
{
	/* hold the system for 15 seconds & display to user that door is unlocking */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is Opening");
	while (g_seconds < DOOR_UNLOCKING_PERIOD);

	/* let the door be open for 3 seconds */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is Now Open");
	while (g_seconds < DOOR_LEFT_OPEN_PERIOD);

	/* hold the system for 15 seconds & display to user that door is locking */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is Closing");
	while (g_seconds < DOOR_UNLOCKING_PERIOD);
}

void sendPasswordViaUART(uint8 * passwordArray)
{
	uint8 cnt;

	for (cnt = 0; cnt < PASSWORD_LENGTH; cnt++)
	{
		UART_sendByte(passwordArray[cnt]);
		_delay_ms(100);
	}
}
