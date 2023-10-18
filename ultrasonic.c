/*
 * ultrasonic.c
 *
 *  Created on: Oct 18, 2023
 *      Author: DELL
 */

#include "ultrasonic.h"
#include <util/delay.h>

static volatile uint8 g_edgeCount = 0;
static volatile uint16 g_timeHigh = 0;

void Ultrasonic_init(void) {
	ICU_ConfigType config = { F_CPU_8, RAISING }; /* Initialize the ICU driver as required*/
	ICU_init(&config);

	ICU_setCallBack(Ultrasonic_edgeProcessing); /* Setup the ICU call back function. */

	/*Setup the direction for the trigger pin as output pin through the GPIO driver. */
	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN,
			PIN_OUTPUT);
}

void Ultrasonic_Trigger(void) {
	/* transmit 10 us trigger pulse */
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void) {
	/*send the ultrasonic sound */
	Ultrasonic_Trigger();

	return ((float) (g_timeHigh / 57.9));
}

void Ultrasonic_edgeProcessing(void) {
	g_edgeCount++;
	if (g_edgeCount == 1) {
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		ICU_clearTimerValue();
		/* Detect falling edge */
		ICU_setEdgeDetectionType(FALLING);
	} else if (g_edgeCount == 2) {
		/* Store the High time value */
		g_timeHigh = ICU_getInputCaptureValue();
		/* Detect rising edge */
		ICU_setEdgeDetectionType(RAISING);
		g_edgeCount = 0;
	}
}
