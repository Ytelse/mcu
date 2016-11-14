#include "em_gpio.h"
#include "em_cmu.h"

#include "led_control.h"

int setupLED(void) {
#ifndef STK
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Enable LED output */

 	  GPIO_PinModeSet(LED_PORT,         	/* Port */
                  	LED_PIN0,         	/* Pin */
                  	gpioModePushPull, 	/* Mode */
                  	0);              	/* Output value */

    GPIO_PinModeSet(LED_PORT,         	/* Port */
                  	LED_PIN1,          	/* Pin */
                  	gpioModePushPull, 	/* Mode */
                  	0);              	/* Output value */

    GPIO_PinModeSet(LED_PORT,         	/* Port */
                  LED_PIN2,          	/* Pin */
                  gpioModePushPull, 	/* Mode */
                  0);	              	/* Output value */

    GPIO_PinModeSet(LED_PORT,         	/* Port */
                  LED_PIN3,          	/* Pin */
                  gpioModePushPull, 	/* Mode */
                  0);    				/* Output value */

    GPIO_PortOutSet(LED_PORT, 0x0000);
#endif
	return 0;
}

int setLEDs(uint32_t mask) {
#ifndef STK
	/* Clear (set low) all outputs */
	GPIO_PortOutClear(LED_PORT, 0xFFFFFFFF);
	/* Set all outputs denoted by mask to high */
	GPIO_PortOutSet(LED_PORT, mask);
#endif
	return 0;
}