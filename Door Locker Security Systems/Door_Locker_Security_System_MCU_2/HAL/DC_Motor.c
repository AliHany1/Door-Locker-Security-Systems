/*********************************************
 *  File Name: DC_Motor.c                   *
 *                                          *
 *  Created on: Oct 5, 2021
 *
 *  Author: Ali Hany                        *
 *********************************************/

#include "DC_Motor.h"
#include "gpio.h"



/*******************************************************************************
 *                         Function Definitions                                *
 *******************************************************************************/

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN1,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN2 ,PIN_OUTPUT);
	GPIO_writePin(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN1,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN2 ,LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state)
{
	switch(state)
	{
	case Stop:
		// Stop the motor
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN1,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN2,LOGIC_LOW);
		break;
	case CW:
		// Rotate the motor --> clock wise
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN1,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN2,LOGIC_HIGH);
		break;
	case CCW:
		// Rotate the motor --> anti-clock wise
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN1,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN2,LOGIC_LOW);
		break;



	}

}

