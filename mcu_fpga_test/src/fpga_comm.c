#include <stdbool.h>

#include "fpga_comm.h"

#include "em_cmu.h"
//#include "em_gpio.h"

#include "defs.h"

#include "leds.h"

/* IMAGE BUFFER POINTERS */
extern uint8_t* img_buf0;
extern uint8_t* img_buf1;
/* BUFFER STATUS VARIABLES */
extern unsigned int buf_idx;
extern uint8_t  	buf_sel;
extern bool 		buf_full;

/* Function prototype */
static void read_bus_data(void);

void setup_FPGA_comm() {
	/* Initialize GPIO */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Initialize GPIO pins */

	/* READY signal -- out */
	GPIO_PinModeSet(DBUS_CTRL_PORT, DBUS_CTRL_PIN_RDY, gpioModePushPull, 0);
	/*   ACK signal -- out */
	GPIO_PinModeSet(DBUS_CTRL_PORT, DBUS_CTRL_PIN_ACK, gpioModePushPull, 0);
	/* VALID signal -- in  */
	GPIO_PinModeSet(DBUS_CTRL_PORT, DBUS_CTRL_PIN_VALID, gpioModeInput, 0);
	/*  DATA signals -- in */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN0, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN1, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN2, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN3, gpioModeInput, 0);

	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN4, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN5, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN6, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN7, gpioModeInput, 0);

	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN8, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN9, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN10, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN11, gpioModeInput, 0);

	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN12, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN13, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN14, gpioModeInput, 0);
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN15, gpioModeInput, 0);

	/* Configure interrupts from VALID signal */
	GPIO_IntConfig(DBUS_CTRL_PORT,			/* GPIO port */
					DBUS_CTRL_PIN_VALID,	/* GPIO pin */
		 			true, 					/* risingEdge */
  		 			false, 					/* fallingEdge */
  		 			true);  				/* enable */
}

void start_FPGA_comm() {

	/** 
	 * Enable interrupts after all other setup is done,
	 * as this could in theory generate an interrupt
	 * before we are done configuring.
	 */

	set_LED(LED2_ON);
	/* Clear any pending odd GPIO interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	/* Enable interrupts on odd GPIO pins */
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	/* Set READY high */
	GPIO_PinOutSet(PIN_READY.port, PIN_READY.pin);
}

void resume_FPGA_comm() {
	/* Set READY high */
	GPIO_PinOutSet(PIN_READY.port, PIN_READY.pin);
}	

void stop_FPGA_comm() {
	set_LED(LED3_ON);
	/* Set READY low */
	GPIO_PinOutClear(PIN_READY.port, PIN_READY.pin);
	/* Disable interrupts */
	NVIC_DisableIRQ(GPIO_ODD_IRQn);
}

static void read_bus_data(void) {
	/* Set READY low */
	GPIO_PinOutClear(PIN_READY.port, PIN_READY.pin);

	uint16_t temp_data = 0;

	/* I think this cast is fine, maybe need to do some testing */
	temp_data = (uint16_t) GPIO_PortInGet(DATA_BUS_PORT); 

	/* All data read, set ACK high */
	GPIO_PinOutSet(PIN_ACK.port, PIN_ACK.pin);

	if (buf_sel) { 	/* Use image buffer 1 */
		img_buf1[buf_idx++] = (uint8_t) (temp_data >> 8); 	/* Upper byte */
		img_buf1[buf_idx++] = (uint8_t) temp_data;			/* Lower byte */
	} else {		/* Use image buffer 0 */
		img_buf0[buf_idx++] = (uint8_t) (temp_data >> 8); 	/* Upper byte */
		img_buf0[buf_idx++] = (uint8_t) temp_data;			/* Lower byte */
	}

	if (buf_idx == BUFFERSIZE_SEND) {
		buf_full = true;
	}

	/* Everything done, set ACK low and READY high */
	GPIO_PinOutClear(PIN_ACK.port, PIN_ACK.pin);
	/**
	 * Only set READY high if there is more room in the buffer. If the 
	 * buffer is full we have to switch buffer before setting READY high again.
	 */ 
	if (!buf_full) {
		GPIO_PinOutSet(PIN_READY.port, PIN_READY.pin);
	}
}

void GPIO_ODD_IRQHandler(void) {
	set_LED(LED3_ON);
	/* As long as we only have one interrupt enabled, this should be safe to use */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	read_bus_data();
}

/* Test function */

uint32_t display_bus_on_led(void) {
	uint32_t temp_data = GPIO_PortInGet(DATA_BUS_PORT);
	// uint32_t led_mask = LEDS_ALL_OFF;

	// if (((temp_data & 0x0000F000) >> 12) > 7) {
	// 	led_mask |= LED0_ON;
	// }

	// if (((temp_data & 0x00000F00) >> 8) > 7) {
	// 	led_mask |= LED1_ON;
	// }

	// if (((temp_data & 0x000000F0) >> 4) > 7) {
	// 	led_mask |= LED2_ON;
	// }

	// if (((temp_data & 0x0000000F) >> 0) > 7) {
	// 	led_mask |= LED3_ON;
	// }

	//set_LED(temp_data << 4);

	// if (GPIO_PortInGet(DATA_BUS_PORT) & 0x00008000) {
	// 	GPIO_PortOutSet(LED_PORT, LEDS_ALL_ON);
	// } else {
	// 	GPIO_PortOutClear(LED_PORT, LEDS_ALL_ON);
	// }
	GPIO_PinOutSet(PIN_ACK.port, PIN_ACK.pin);
	GPIO_PinOutClear(PIN_ACK.port, PIN_ACK.pin);
	return temp_data << 4;
}	

/* Test function */

void set_ack_low(void) {
	GPIO_PinOutClear(PIN_ACK.port, PIN_ACK.pin);
}

