#include "dbus.h"

/* Initialize the DBUS */

void DBUS_init(void) {
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

	/* Configure VALID signal to trigger interrupts */
	GPIO_IntConfig(DBUS_CTRL_PORT,			/* GPIO port */
					DBUS_CTRL_PIN_VALID,	/* GPIO pin */
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

void DBUS_stop(void) {
	/* Disable GPIO interrupts */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_DisableIRQ(GPIO_ODD_IRQn);
	/* Signal FPGA that we will no longer read any data */
	DBUS_clear_READY();
}
