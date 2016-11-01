#include "em_usb.h"

#include "usb_control.h"
#include "usb_callbacks.h"
#include "usb_descriptors.h"

int setupUSB(void) {
	USBD_Init(&initstruct);
	return 0;
}