#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_gpio.h"

#include "defs.h"

#include "usbcallbacks.h"
#include "leds.h"

UBUF(recv_buf, BUFFERSIZE_RECV);
/* BUFFER STATUS VARIABLE(S) */
extern bool		buf_rdy;
/* Wait for ready from USB host */
extern bool _wait;
extern bool _halt;

void stateChange(USBD_State_TypeDef oldState, USBD_State_TypeDef newState) {
	UNUSED(oldState);

	if (newState == USBD_STATE_CONFIGURED) {
		set_LED(LED0_ON | LED1_ON);
		memset(recv_buf, 0, BUFFERSIZE_RECV);
		USBD_Read(EP_OUT, recv_buf, BUFFERSIZE_RECV, dataReceivedCallback);
	} else if (newState == USBD_STATE_ADDRESSED) {
		set_LED(LED0_ON);
	}
}

int dataSentCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	
	UNUSED(xferred);
	UNUSED(remaining);

	if (status == USB_STATUS_OK) {
		/* Data successfully transferred. This means currently inactive buffer is ready for more data. */
		buf_rdy = true;
	}

	set_LED(LED1_ON);

	return USB_STATUS_OK;
}

int dataReceivedCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	UNUSED(xferred);
	UNUSED(remaining);

	if (status == USB_STATUS_OK) {
		if (_wait) {
			_wait = false;
			set_LED(LED0_ON | LED2_ON);
			USBD_Read(EP_OUT, recv_buf, BUFFERSIZE_RECV, dataReceivedCallback);
		} else if (!_halt) {
			_halt = true;
			set_LED(LED0_ON | LED3_ON);
		}
	}

	return USB_STATUS_OK;
}