#include "dbus.h"

#include "em_cmu.h"
#include "em_gpio.h"

/* Initialize the DBUS */

void DBUS_init(void) {
	/* Initialize GPIO */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Initialize GPIO pins */

	/* CONTROL signals */
	GPIO_PinModeSet(DBUS_RDY_PORT, DBUS_RDY_PIN, gpioModePushPull, 0); 	/* Out */
	GPIO_PinModeSet(DBUS_ACK_PORT, DBUS_ACK_PIN, gpioModePushPull, 0); 	/* Out */
	GPIO_PinModeSet(DBUS_VLD_PORT, DBUS_VLD_PIN, gpioModeInput, 0);		/* In */
	
	/*  DATA signals -- in */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN0, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN1, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN2, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN3, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN4, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN5, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN6, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN7, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN8, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN9, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN10, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN11, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN12, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN13, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN14, gpioModeInput, 0);	/* In */
	GPIO_PinModeSet(DBUS_DATA_PORT, DBUS_DATA_PIN15, gpioModeInput, 0);	/* In */

	/* Configure VALID signal to trigger interrupts */
	GPIO_IntConfig(DBUS_VLD_PORT,			/* GPIO port */
					DBUS_VLD_PIN,	/* GPIO pin */
		 			true, 					/* risingEdge */
  		 			false, 					/* fallingEdge */
  		 			true);  				/* enable */
	
}

/* ======================================================== */
/* =           APPLICATION SPECIFIC FUNCTIONS             = */
/* ======================================================== */

/* Start communication with FPGA */

void DBUS_start(void) {
	/* Enable GPIO interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	/* Signal FPGA that we are ready to receive data */
	DBUS_set_READY();
}

/* Completely stop communication with FPGA */

void DBUS_stop(void) {
	/* Disable GPIO interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_DisableIRQ(GPIO_ODD_IRQn);
	/* Signal FPGA that we will no longer read any data */
	DBUS_clear_READY();
}
