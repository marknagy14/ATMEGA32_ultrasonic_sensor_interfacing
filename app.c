/*
 * app.c
 *
 *  Created on: Oct 18, 2023
 *      Author: DELL
 */

#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>

int main(void) {
	uint16 distance = 0;

	/*Enable interrupts by setting I-bit*/
	SREG |= (1<<7);
	Ultrasonic_init();
	LCD_init();
	LCD_displayString("distance =   cm");

	while (1) {
		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0,10);
		if (distance >= 100) {
			LCD_intgerToString(distance);
		} else {
			LCD_intgerToString(distance);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}

	}
}

