/*
 * Timer0.c
 *
 *  Created on: Oct 18, 2021
 *      Author: Ali Hany
 */

#include "Timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*g_Timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2CallBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if (g_Timer0CallBackPtr != NULL_PTR)
	{
		(*g_Timer0CallBackPtr)();
	}
}

/* Timer0 compare */
ISR(TIMER0_COMP_vect)
{
	if (g_Timer0CallBackPtr != NULL_PTR)
	{
		(*g_Timer0CallBackPtr)();
	}
}

/* Timer1 overflow*/
ISR(TIMER1_OVF_vect)
{
	if (g_Timer1CallBackPtr != NULL_PTR)
	{
		(*g_Timer1CallBackPtr)();
	}
}

/* Timer1 compare */
ISR(TIMER1_COMPA_vect)
{
	if (g_Timer1CallBackPtr != NULL_PTR)
	{
		(*g_Timer1CallBackPtr)();
	}
}

/* Timer2 overflow*/
ISR(TIMER2_OVF_vect)
{
	if (g_Timer2CallBackPtr != NULL_PTR)
	{
		(*g_Timer2CallBackPtr)();
	}
}

/* Timer2 compare */
ISR(TIMER2_COMP_vect)
{
	if (g_Timer2CallBackPtr != NULL_PTR)
	{
		(*g_Timer2CallBackPtr)();
	}
}
void Timer2_setCallBack(void(*a_ptr)(void))
{
	g_Timer2CallBackPtr = a_ptr;
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_Timer1CallBackPtr = a_ptr;
}

void Timer0_setCallBack(void(*a_ptr)(void))
{
	g_Timer2CallBackPtr = a_ptr;
}
void Timer_init(const TIMER_Config *Config_ptr)
{
if (Config_ptr -> Timer_ID == Timer0)
{
		/* starting with adjusting common bits for the timer, then mode-specific bits */

		SET_BIT(TCCR0,FOC0); /* for non-PWM */
		TCNT0 =Config_ptr -> Initial_Value;
		TCCR0 = 0;
		TCCR0 |= (Config_ptr -> clk_selection);

		if (Config_ptr -> mode == NORMAL)
		{
			SET_BIT(TIMSK,TOIE0); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV0); /* enable overflow flag */

		}
		else if (Config_ptr -> mode == CTC)
		{
			SET_BIT(TCCR0,WGM01);
			OCR0 = Config_ptr -> CompareValue;
			SET_BIT(TIMSK,OCIE0); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF0); /* enable CTC flag */
		}


	}
else if (Config_ptr -> Timer_ID == Timer1)
	{
		TCCR1A =0;
		SET_BIT(TCCR1A,FOC1A); /* for non-PWM mode */
		SET_BIT(TCCR1A,FOC1B);
		TCCR1B = Config_ptr -> clk_selection;
		TCNT1 = Config_ptr -> Initial_Value;

		if (Config_ptr -> mode == NORMAL)
		{
			SET_BIT(TIMSK, TOIE1); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV1);

		}
		else if (Config_ptr -> mode == CTC)
		{
			SET_BIT(TCCR1B, WGM12);
			OCR1A = Config_ptr -> CompareValue;
			SET_BIT(TIMSK, OCIE1A); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF1A); /* enable CTC flag */
		}
	}
else if (Config_ptr -> Timer_ID == Timer2)
{
		SET_BIT(TCCR2, FOC2); /* for non-PWM */
		TCNT2 = Config_ptr -> Initial_Value;
		TCCR2 = 0;
		TCCR2 |= (Config_ptr -> clk_selection);

		if (Config_ptr -> mode == NORMAL)
		{
			SET_BIT(TIMSK, TOIE2); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV2); /* enable overflow flag */

		} else if (Config_ptr -> mode == CTC)
		{
			SET_BIT(TCCR0, WGM21);
			OCR2 = Config_ptr -> CompareValue;
			SET_BIT(TIMSK, OCIE2); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF2); /* enable CTC flag */
		}
	}
}


void Timer_deInit(TIMER_ID timer_ID)
{
	if (timer_ID == Timer0)
	{
		TCCR0 = 0;
		TCNT0= 0; /* clear initial value */
		OCR0 = 0; /* clear compare value for CTC mode*/
		CLEAR_BIT(TIMSK,OCIE0); /* disable interrupts for CTC mode */
		CLEAR_BIT(TIMSK,TOIE0); /* disable interrupts for overflow mode */
		g_Timer0CallBackPtr = NULL_PTR; /* clear the call-back function */

	}
	else if (timer_ID == Timer1)
	{
		TCCR1A =0;
		TCCR1B = 0; /* disable clock */
		TCNT1 = 0;
		CLEAR_BIT(TIMSK, TOIE1); /* disable interrupts for overflow mode */
		OCR1A = 0;  /* clear compare value for CTC mode */
		CLEAR_BIT(TIMSK, OCIE1A); /* disable interrupts for CTC mode */
		g_Timer1CallBackPtr = NULL_PTR;

	}
	else if (timer_ID == Timer2)
	{
		TCCR2 = 0;
		TCNT2 = 0; /* clear initial value */
		OCR2 = 0; /* clear compare value for CTC mode*/
		CLEAR_BIT(TIMSK, OCIE2); /* disable interrupts for CTC mode */
		CLEAR_BIT(TIMSK, TOIE2); /* disable interrupts for overflow mode */
		g_Timer2CallBackPtr = NULL_PTR;
	}
}

