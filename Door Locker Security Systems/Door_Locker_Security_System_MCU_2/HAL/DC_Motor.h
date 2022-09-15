/*********************************************
 *  File Name: DC_Motor.h                   *
 *                                          *
 *  Created on: Oct 5, 2021
 *
 *  Author: Ali Hany                        *
 *********************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	Stop,CW,CCW
}DcMotor_State;

/*******************************************************************************
 *                         Definitions                                         *
 *******************************************************************************/

#define DC_MOTOR_PORT_ID            PORTC_ID
#define DC_MOTOR_PIN_IN1            PIN3_ID
#define DC_MOTOR_PIN_IN2            PIN4_ID


/*******************************************************************************
 *                         Function Prototypes                                 *
 *******************************************************************************/

void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state);

#endif /* DC_MOTOR_H_ */
