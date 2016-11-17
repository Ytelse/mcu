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

//#include "rtc_timing.h"

/* UBUF is a macro for creating WORD-aligned uint8_t buffers */
UBUF(img_buf0, BUFFERSIZE_SEND);
UBUF(img_buf1, BUFFERSIZE_SEND);

volatile bool _wait, _halt;

volatile uint32_t msTicks;
// volatile uint32_t bus_reads;

/* RTC Interrupt Handler */

// void RTC_IRQHandler(void) {
// 	RTC_IntClear(RTC_IFC_COMP0);

// 	uint32_t led_mask = 0;
// 	led_mask |= (bus_reads > 0x000E0000) ? LED0 : LEDS_NONE;
// 	led_mask |= (bus_reads > 0x000C0000) ? LED1 : LEDS_NONE;
// 	led_mask |= (bus_reads > 0x000A0000) ? LED2 : LEDS_NONE;
// 	led_mask |= (bus_reads > 0x00080000) ? LED3 : LEDS_NONE;

// 	LEDS_update_all(led_mask);
// 	bus_reads = 0;
// }

/* SystTick Handler */

void SysTick_Handler(void) {
	msTicks++;       /* increment counter necessary in Delay()*/

}

void Delay(uint32_t dlyTicks) {
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) ;
}

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

	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

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
	static MSTATE_init_struct_t mstate_initstruct = {
		.mcuState	=	MSTATE_MCU_WAIT,
		.bufSelect	=	MSTATE_BUF_SEL_0
	};

	MSTATE_init(&mstate_initstruct);

	/* Enable Real-time clock interrupt every 1 second */
	//setupRtc();

	LEDS_clear_all();

	while (msTicks < 10000) {
		/* Set LEDs to APSR[31:28] */ 
		LEDS_update_all(__MSTATE_get_all() >> 28);
		Delay(200);
	}

	while (msTicks < 20000) {
		/* Set LEDs to APSR[27:24] */
		LEDS_update_all(__MSTATE_get_all() >> 24);
		Delay(200);
	}

	while (msTicks < 30000) {
		/* Set LEDs to APSR[23:20] */
		LEDS_update_all(__MSTATE_get_all() >> 20);
		Delay(200);
	}

	while (msTicks < 40000) {
		/* Set LEDs to APSR[19:16] */
		LEDS_update_all(__MSTATE_get_all() >> 16);
		Delay(200);
	}

	while (msTicks < 50000) {
		/* Set LEDs to APSR[15:12] */
		LEDS_update_all(__MSTATE_get_all() >> 12);
		Delay(200);
	}

	while (msTicks < 60000) {
		/* Set LEDs to APSR[11:8] */
		LEDS_update_all(__MSTATE_get_all() >> 8);
		Delay(200);
	}

	while (msTicks < 70000) {
		/* Set LEDs to APSR[7:4] */
		LEDS_update_all(__MSTATE_get_all() >> 4);
		Delay(200);
	}

	while (msTicks < 80000) {
		/* Set LEDs to APSR[3:0] */
		LEDS_update_all(__MSTATE_get_all() >> 0);
		Delay(200);
	}
}
