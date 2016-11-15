#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_gpio.h"

#include "defs.h"

#include "usbcallbacks.h"
#include "leds.h"

/* IMAGE BUFFER POINTERS */
extern uint8_t* img_buf0;
extern uint8_t* img_buf1;
extern uint8_t* recv_buf;
/* BUFFER STATUS VARIABLES */
extern uint8_t* buf_ptr;
extern uint8_t  buf_sel;
extern bool 	buf_full;

// volatile uint32_t counter = 0;
// volatile bool ledsON = false;


void stateChange(USBD_State_TypeDef oldState, USBD_State_TypeDef newState) {
	(void)oldState;

	
	if (newState == USBD_STATE_CONFIGURED) {
		// printf("%s\n", );
		set_LED(LED0_ON | LED1_ON);
		USBD_Read(EP_OUT, recv_buf, BUFFERSIZE_RECV, dataReceivedCallback);
	} else if (newState == USBD_STATE_ADDRESSED) {
		set_LED(LED0_ON);
	}
}

int dataSentCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	/* TODO: Find out what to do when transfer completes */
	/* TODO: Set a flag denoting current inactive buffer is ready? */
	return USB_STATUS_OK;
}

int dataReceivedCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	(void)xferred; (void)remaining;

	if (status == USB_STATUS_OK) {
		set_LED(LED0_ON | LED1_ON);
		USBD_Read(EP_OUT, recv_buf, BUFFERSIZE_RECV, dataReceivedCallback);
	}

	return USB_STATUS_OK;
}