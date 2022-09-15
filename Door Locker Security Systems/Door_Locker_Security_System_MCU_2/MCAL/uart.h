/*
 * uart.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Ali Hany
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	_5_BIT, _6_BIT, _7_BIT, _8_BIT
}CHARACTER_SIZE;

typedef enum
{
	DISABLED, EVEN = 2, ODD
}PARITY_MODE;

typedef enum
{
	_1_BIT, _2_BIT
}STOP_BIT_SELCTION;

typedef struct
{
	CHARACTER_SIZE Bit_Data;
	PARITY_MODE ParityType;
	STOP_BIT_SELCTION Stop_Bit_Num;
}Uart_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(uint32 baud_rate, Uart_Config *Config_ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #
#endif /* UART_H_ */
