#include <stdbool.h>

#include "em_usb.h"

#include "usbcontrol.h"
#include "usbcallbacks.h"
#include "usbdescriptors.h"

#include "defs.h"

/* BUFFER STATUS VARIABLE(S) */
extern uint8_t buf_sel;

void setup_USB() {
	USBD_Init(&initstruct);
}

void notify_img_buf_ready() {
	/* TODO: Write buffer to host, buffer should be opposite of buf_sel */

}