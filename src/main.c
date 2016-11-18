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

volatile uin32_t state;
/* Address of next available memory location in active image buffer */
volatile uint8_t* ptr;

int main(void) {

	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

	/* Initialize image buffers to 0 */
	memset(img_buf0, 0, BUFFERSIZE_SEND);
	memset(img_buf1, 0, BUFFERSIZE_SEND);

	/* Initialize LED pins */
	LEDS_init();

	/* Setup USB stack */
	USBD_Init(&initstruct);

	/* Initialize DBUS for communication with FPGA */
	DBUS_init();

	/* Set up control variables */
	// static MSTATE_init_struct_t mstate_initstruct = {
	// 	.mcuState	=	MSTATE_MCU_WAIT,
	// 	.bufSelect	=	MSTATE_BUF_SEL_0
	// };

	// MSTATE_init(&mstate_initstruct);

	while (state & MSTATE_MCU_WAIT) {
		/* Wait for msg from USB Host */
		/* Read is set up in usbcallbacks */
	}

	/* Send ready to FPGA and enable interrupts on VALID signal */
	DBUS_start();

	while (state & MSTATE_MCU_RUN) {
		/* DO NOTHING */
		/* Everything should be handled by interrupts */
		/* Do not go into any sleep modes */
	}

	while (1) {

	}
}

void GPIO_ODD_IRQHandler(void) {
	/**
	 * TODO: See definicitions of NVIC_* in CMSIS lib. See if they can 
	 * be optimized (inlining). Believe they already are inlined, but 
	 * worth a check.
	 */

	/**
	 * TODO: Check if we actually need to disable interrupts inside
	 * interrupt routine. Doubt it.
	 */

	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_DisableIRQ(GPIO_ODD_IRQn);
	
	/* Write values on data bus to address specified by ptr */
	DBUS_read_and_store(ptr);

	/* TODO: Keep track of pointer value! If it exceeds buffer space we need to switch buffers */

	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

