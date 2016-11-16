#include <stdbool.h>

#include "em_usb.h"

#include "usbcontrol.h"
#include "usbcallbacks.h"
#include "usbdescriptors.h"

#include "defs.h"

#include "leds.h"

/* IMAGE BUFFER POINTERS */
extern uint8_t* img_buf0;
extern uint8_t* img_buf1;

/* BUFFER STATUS VARIABLE(S) */
extern uint8_t buf_sel;

/* TODO: remove entire file, unneeded function calls take precious cycles */

void setup_USB() {
	USBD_Init(&initstruct);

}

/* TODO: Move this back into main */

void notify_img_buf_ready(void) {
	if (!buf_sel) { /* Write opposite of currently active buffer */
		USBD_Write(EP_IN, img_buf1, BUFFERSIZE_SEND, dataSentCallback);
	} else {
		USBD_Write(EP_IN, img_buf0, BUFFERSIZE_SEND, dataSentCallback);
	}

	set_LED(LED0_ON);

}