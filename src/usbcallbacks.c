#include "em_device.h"
#include "em_chip.h"
#include "em_usb.h"

#include "defs.h"

#include "usbcallbacks.h"
#include "leds.h"
#include "mstate.h"

UBUF(recv_buf, BUFFERSIZE_RECV);

extern uint32_t MSTATE;

void stateChange(USBD_State_TypeDef oldState, USBD_State_TypeDef newState) {
	UNUSED(oldState);

	if (newState == USBD_STATE_CONFIGURED) {
		LEDS_set(LED1);
		memset(recv_buf, 0, BUFFERSIZE_RECV);
		USBD_Read(EP_OUT, recv_buf, BUFFERSIZE_RECV, dataReceivedCallback);
	} else if (newState == USBD_STATE_ADDRESSED) {
		LEDS_set(LED0);
	}
}

int dataSentCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	
	UNUSED(xferred);
	UNUSED(remaining);

	if (status == USB_STATUS_OK) {
		if (MSTATE & MSTATE_BUF_SEL) {
			MSTATE |= MSTATE_BUF_1_RDY;
			LEDS_set(LED1);
			LEDS_clear(LED0);
		} else {
			MSTATE |= MSTATE_BUF_0_RDY;
			LEDS_set(LED0);
			LEDS_clear(LED1);
		}
	}

	return USB_STATUS_OK;
}

int dataReceivedCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	UNUSED(xferred);
	UNUSED(remaining);

	if (status == USB_STATUS_OK) {
		if (MSTATE & MSTATE_MCU_WAIT) {
			LEDS_update_all(LED2);
			MSTATE |= MSTATE_MCU_RUN;
			MSTATE &= ~MSTATE_MCU_WAIT;
			USBD_Read(EP_OUT, recv_buf, BUFFERSIZE_RECV, dataReceivedCallback);
		} else if (MSTATE & MSTATE_MCU_RUN) {
			MSTATE |= MSTATE_MCU_HALT;
			MSTATE &= ~MSTATE_MCU_RUN;
		}
	}

	return USB_STATUS_OK;
}