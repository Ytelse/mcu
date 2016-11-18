#include "em_gpio.h"
#include "em_cmu.h"

#include "leds.h"

void LEDS_init(void) {
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
}