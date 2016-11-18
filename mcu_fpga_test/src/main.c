#include <stdbool.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_gpio.h"
#include "em_rtc.h"
#include "em_emu.h"

#include "defs.h"

#include "leds.h"
#include "dbus.h"
#include "mstate.h"

#include "usbcallbacks.h"
#include "usbdescriptors.h"

#include "rtc_timing.h"

/* UBUF is a macro for creating WORD-aligned uint8_t buffers */
UBUF(img_buf0, BUFFERSIZE_SEND);
UBUF(img_buf1, BUFFERSIZE_SEND);

volatile bool _wait, _halt;

volatile uint32_t msTicks;
volatile uint32_t bus_reads;

/* RTC Interrupt Handler */

void RTC_IRQHandler(void) {
	RTC_IntClear(RTC_IFC_COMP0);


	uint32_t led_mask = 0;
	led_mask |= (bus_reads > 0x00186A00) ? LED0 : LEDS_NONE; // 1 600 000
	led_mask |= (bus_reads > 0x001842F0) ? LED1 : LEDS_NONE; // 1 590 000
	led_mask |= (bus_reads > 0x00181BE0) ? LED2 : LEDS_NONE; // 1 580 000
	led_mask |= (bus_reads > 0x0017F4D0) ? LED3 : LEDS_NONE; // 1 570 000

	LEDS_update_all(led_mask);
	bus_reads = 0;
}

/* SystTick Handler */

// void SysTick_Handler(void) {
// 	msTicks++;       /* increment counter necessary in Delay()*/

// }

// void Delay(uint32_t dlyTicks) {
// 	uint32_t curTicks;
// 	curTicks = msTicks;
// 	while ((msTicks - curTicks) < dlyTicks) ;
// }

// void switch_buf(void) {
// 	buf_sel = (buf_sel) ? 0 : 1; 	/* Select the buffer that is not in use */
// 	buf_idx = 0; 					/* Reset buf_idx */
// 	buf_full = false;				/* New buffer is not full */
// 	buf_rdy = false;				/* Inactive buffer is not ready */

// 	DBUS_resume();
// }

int main(void) {

	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

	// if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

	/* Initialize image buffers to 0 */
	// memset(img_buf0, 0, BUFFERSIZE_SEND);
	// memset(img_buf1, 0, BUFFERSIZE_SEND);

	/* Initialize LED pins */
	LEDS_init();

	/* Setup USB stack */
	// USBD_Init(&initstruct);

	/* Initialize DBUS for communication with FPGA */
	DBUS_init();

	/* Set up control variables */
	// static MSTATE_init_struct_t mstate_initstruct = {
	// 	.mcuState	=	MSTATE_MCU_WAIT,
	// 	.bufSelect	=	MSTATE_BUF_SEL_0
	// };

	// MSTATE_init(&mstate_initstruct);

	// Enable Real-time clock interrupt every 1 second */
	bus_reads = 0;
	setupRtc();

	LEDS_clear_all();

	//DBUS_start();

	/* LOGIC ANALYZER TRIGGER */

	GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 1);
	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);
	GPIO_PinOutClear(gpioPortA, 0);
	GPIO_PinOutClear(gpioPortA, 1);

	GPIO_PinOutSet(gpioPortA, 0);
	GPIO_PinOutSet(gpioPortA, 1);

	/* END LOGIC ANALYZER TRIGGER */

	DBUS_set_READY();

	while (1) {
		if (DBUS_get_VALID()) {
			DBUS_read();
			bus_reads++;
		} else {
			break;
		}
	}

	while (1) {
		/* Trap */
		LEDS_update_all(LED0);
	}
}

// void GPIO_ODD_IRQHandler(void) {
// 	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
// 	NVIC_DisableIRQ(GPIO_ODD_IRQn);
// 	LEDS_update_all(DBUS_read() << 4);
// 	NVIC_EnableIRQ(GPIO_ODD_IRQn);
// }

