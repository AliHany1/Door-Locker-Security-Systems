/*
 * Timer0.h
 *
 *  Created on: Oct 18, 2021
 *      Author: ALi Hany
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/


#define NULL_PTR    ((void*)0)

typedef enum{
	Timer0, Timer1, Timer2
}TIMER_ID;

typedef enum
{
	NORMAL, CTC = 2
}TIMER_MODE;

typedef enum
{
    NO_CLK, F_CPU_1, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}TIMER_CLK_SELECT;

typedef struct
{
	TIMER_ID Timer_ID;
	TIMER_MODE mode;
	TIMER_CLK_SELECT clk_selection;
	uint16 CompareValue;
	uint16 Initial_Value;
}TIMER_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Timer_init(const TIMER_Config *Config_ptr);

void Timer_DeInit(TIMER_ID timer_ID);

void Timer2_setCallBack(void(*a_ptr)(void));

void Timer1_setCallBack(void(*a_ptr)(void));

void Timer0_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
