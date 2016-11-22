#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"

#include "defs.h"

#include "leds.h"
#include "dbus.h"
#include "mstate.h"

#include "usbcallbacks.h"
#include "usbdescriptors.h"


/* WORD-aligned image buffers */

UBUF(img_buf0, BUFFERSIZE_SEND);
UBUF(img_buf1, BUFFERSIZE_SEND);

/* MCU state */

volatile uint32_t MSTATE;

int main(void) {

	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

	/* Initialize image buffers to 0 */
	memset(img_buf0, 0, BUFFERSIZE_SEND);
	memset(img_buf1, 0, BUFFERSIZE_SEND);

	/* Initialize LED pins */
	LEDS_init();
	LEDS_set(LED3);

	/* Setup USB stack */
	USBD_Init(&initstruct);

	/* Initialize DBUS for communication with FPGA */
	DBUS_init();

	/* Set initial state */
	MSTATE = (MSTATE_MCU_WAIT | MSTATE_BUF_0_RDY | MSTATE_BUF_1_RDY | MSTATE_CUR_BUF_RDY);

	LEDS_set(LED2);

	while (MSTATE & MSTATE_MCU_WAIT) {
		/* Wait until notified by USB host */
	}

	LEDS_clear(LED3);

	/* Address of next available memory location in active image buffer */
	uint8_t* ptr = img_buf0;
	/* Because there is no guarantee that the image buffers are contiguous in memory we have to keep track of the idx seperately */
	uint32_t idx = 0;

	/* Send ready to FPGA in order to initiate communication */
	DBUS_set_READY();

	while (MSTATE & MSTATE_MCU_RUN) {
		if (DBUS_get_VALID() && (MSTATE & (MSTATE_BUF_0_RDY))) { // | MSTATE_BUF_1_RDY))) {
			uint16_t data = DBUS_read();
			ptr[idx++] = (uint8_t) (data >> 8);
			ptr[idx++] = (uint8_t) (data & 0x00FF);
			
			if (idx == BUFFERSIZE_SEND) {
				USBD_Write(EP_IN, img_buf0, BUFFERSIZE_SEND, dataSentCallback);
				MSTATE &= ~MSTATE_BUF_0_RDY;
				idx = 0;
			}
		}

		// if (idx == BUFFERSIZE_SEND) {
		// 	USBD_Write(EP_IN, img_buf0, BUFFERSIZE_SEND, dataSentCallback);
		// 	MSTATE &= ~MSTATE_BUF_0_RDY;
		// 	if (MSTATE & MSTATE_BUF_SEL) {
		// 		USBD_Write(EP_IN, img_buf1, BUFFERSIZE_SEND, dataSentCallback);
		// 		MSTATE &= ~(MSTATE_BUF_1_RDY | MSTATE_BUF_SEL);
		// 		ptr = img_buf0;
		// 	} else {
		// 		USBD_Write(EP_IN, img_buf0, BUFFERSIZE_SEND, dataSentCallback);
		// 		MSTATE &= ~MSTATE_BUF_0_RDY;
		// 		MSTATE |= MSTATE_BUF_SEL;
		// 		ptr = img_buf1;
		// 	}
		// 	idx = 0;
		// }
	}

	while (1) {
		LEDS_set(LED3);
	}
}
