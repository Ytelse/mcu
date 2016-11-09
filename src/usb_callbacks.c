#include <string.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_gpio.h"

#include "usb_callbacks.h"
#include "main.h"

#define BUFFERSIZE 512

/* Buffer to receive incoming messages. Needs to be 
 * WORD aligned and an integer number of WORDs large */
STATIC_UBUF(receiveBuffer, BUFFERSIZE);

void resetCmd(void) {
  	/* Called whenever device receives USB reset signal */
}

int setupCmd(const USB_Setup_TypeDef *setup) {
	/* Called whenever device receives setup request */
  	return USB_STATUS_REQ_UNHANDLED;
}

void stateChange(USBD_State_TypeDef oldState, USBD_State_TypeDef newState) {
	/* Called whenever USB state chages */
}

int dataSentCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	/* Called whenever a data transfer (device to host) completes (succeeds or fails) */
  	return USB_STATUS_OK;
}

int dataReceivedCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining) {
	/* Called whenever a data transfe (host to device) completes (succeeds or fails) */

  /* Remove warnings for unused variables */
  (void)xferred;
  (void)remaining;

  if ( status == USB_STATUS_OK ) {
    if (strcmp((char*) receiveBuffer, "run") == 0) {
      if (state.mcu_state == IDLE) {
	/* Wake the MCU and start transfer */
	state.mcu_state = RUN;
      } else {
	/* ERROR */
      }
    }
  }

  return USB_STATUS_OK;
}
