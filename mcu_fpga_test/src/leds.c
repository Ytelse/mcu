#include "em_gpio.h"
#include "em_cmu.h"

#include "leds.h"

int setup_LED(void) {
	/* Enable GPIO clock */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Enable LED output */

	GPIO_PinModeSet(LED_PORT,
					LED_PIN0,
					gpioModePushPull,
					0);

	GPIO_PinModeSet(LED_PORT,
					LED_PIN1,
					gpioModePushPull,
					0);

	GPIO_PinModeSet(LED_PORT,
					LED_PIN2,
					gpioModePushPull,
					0);

	GPIO_PinModeSet(LED_PORT,
					LED_PIN3,
					gpioModePushPull,
					0);

	return 0;
}

int set_LED(uint32_t mask) {
	/* Clear all output */
	GPIO_PortOutClear(LED_PORT, 0xFFFFFFFF);
	/* Set mask to high */
	GPIO_PortOutSet(LED_PORT, mask);
	return 0;
}