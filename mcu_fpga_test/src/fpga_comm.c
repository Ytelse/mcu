#include <stdbool.h>

#include "fpga_comm.h"

#include "em_cmu.h"
//#include "em_gpio.h"

#include "defs.h"

/* CONTROL */
static const port_pin_t PIN_READY = (port_pin_t){.port = gpioPortC, .pin = 8};  		// EBI.C6
static const port_pin_t PIN_VALID = (port_pin_t){.port = gpioPortC, .pin = 9};  		// EBI.C7
static const port_pin_t PIN_ACK   = (port_pin_t){.port = gpioPortC, .pin = 10}; 		// EBI.C8
/* BYTE 0 */
static const port_pin_t PIN_DATA0 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 0};		// EBI.A7
static const port_pin_t PIN_DATA1 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 1};		// EBI.A8
static const port_pin_t PIN_DATA2 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 2};		// EBI.A9
static const port_pin_t PIN_DATA3 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 3};		// EBI.A10
/* BYTE 1 */
static const port_pin_t PIN_DATA4 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 4};		// EBI.A11
static const port_pin_t PIN_DATA5 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 5};		// EBI.A12
static const port_pin_t PIN_DATA6 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 6};		// EBI.A13
static const port_pin_t PIN_DATA7 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 7};		// EBI.A14
/* BYTE 2 */
static const port_pin_t PIN_DATA8 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 8};		// EBI.D0
static const port_pin_t PIN_DATA9 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 9};		// EBI.D1
static const port_pin_t PIN_DATA10 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 10};	// EBI.D2
static const port_pin_t PIN_DATA11 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 11};	// EBI.D3
/* BYTE 3 */
static const port_pin_t PIN_DATA12 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 12};	// EBI.D4
static const port_pin_t PIN_DATA13 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 13};	// EBI.D5
static const port_pin_t PIN_DATA14 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 14};	// EBI.D6
static const port_pin_t PIN_DATA15 = (port_pin_t){.port = DATA_BUS_PORT, .pin = 15};	// EBI.D7

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
	GPIO_PinModeSet(PIN_READY.port, PIN_READY.pin, gpioModePushPull, 0);
	/*   ACK signal -- out */
	GPIO_PinModeSet(PIN_ACK.port, PIN_ACK.pin, gpioModePushPull, 0);
	/* VALID signal -- in  */
	GPIO_PinModeSet(PIN_VALID.port, PIN_VALID.pin, gpioModeInput, 0);
	/*  DATA signals -- in */
	GPIO_PinModeSet(PIN_DATA0.port, PIN_DATA0.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA1.port, PIN_DATA1.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA2.port, PIN_DATA2.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA3.port, PIN_DATA3.pin, gpioModeInput, 0);

	GPIO_PinModeSet(PIN_DATA4.port, PIN_DATA4.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA5.port, PIN_DATA5.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA6.port, PIN_DATA6.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA7.port, PIN_DATA7.pin, gpioModeInput, 0);

	GPIO_PinModeSet(PIN_DATA8.port, PIN_DATA8.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA9.port, PIN_DATA9.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA10.port, PIN_DATA10.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA11.port, PIN_DATA11.pin, gpioModeInput, 0);

	GPIO_PinModeSet(PIN_DATA12.port, PIN_DATA12.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA13.port, PIN_DATA13.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA14.port, PIN_DATA14.pin, gpioModeInput, 0);
	GPIO_PinModeSet(PIN_DATA15.port, PIN_DATA15.pin, gpioModeInput, 0);

	/** 
	 * Enable interrupts after all other setup is done,
	 * as this could in theory generate an interrupt
	 * before we are done configuring.
	 */

	/* Enable interrupts on odd GPIO pins */
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	/* Configure interrupts from VALID signal */
	GPIO_IntConfig(PIN_VALID.port,	/* GPIO port */
					PIN_VALID.pin,	/* GPIO pin */
		 			true, 			/* risingEdge */
  		 			false, 			/* fallingEdge */
  		 			true);  		/* enable */
	/* Clear any pending odd GPIO interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
}

void start_FPGA_comm() {
	/* Set READY high */
	GPIO_PinOutSet(PIN_READY.port, PIN_READY.pin);
}

void stop_FPGA_comm() {
	/* Set READY low */
	GPIO_PinOutClear(PIN_READY.port, PIN_READY.pin);
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
	/* As long as we only have one interrupt enabled, this should be safe to use */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	read_bus_data();
}