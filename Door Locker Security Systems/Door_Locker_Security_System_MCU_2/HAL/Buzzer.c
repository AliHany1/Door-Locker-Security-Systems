/*
 * Buzzer.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Ali Hany
 */

#include "Buzzer.h"
#include "gpio.h"

void BZR_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
	BZR_OFF();
}

void BZR_ON(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

void BZR_OFF(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

