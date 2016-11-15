#include <stdbool.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"

#include "defs.h"

#include "usbcontrol.h"
#include "leds.h"
#include "fpga_comm.h"

/* UBUF is a macro for creating WORD-aligned uint8_t buffers */
UBUF(img_buf0, BUFFERSIZE_SEND);
UBUF(img_buf1, BUFFERSIZE_SEND);
UBUF(recv_buf, BUFFERSIZE_RECV);
/* BUFFER STATUS VARIABLES */
volatile unsigned int 	buf_idx; 	/* Index of the first unwritten memory location of active buffer */
volatile uint8_t 		buf_sel; 	/* Denotes which buffer is currently active, 0 -> buf0, 1 -> buf1 */
volatile bool 			buf_full; 	/* Denotes whether active buffer is full or not */
volatile bool			buf_rdy;	/* Denotes whether inactive buffer is finished being written over USB */

volatile uint32_t msTicks;

void SysTick_Handler(void) {
	msTicks++;
}

void Delay(uint32_t dlyTicks) {
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) ;
}

int main(void) {
	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

	buf_idx = 0;
	buf_sel = 0;
	buf_full = false;
	buf_rdy = true;

	memset(img_buf0, 0, BUFFERSIZE_SEND);
	memset(img_buf1, 0, BUFFERSIZE_SEND);
	memset(recv_buf, 0, BUFFERSIZE_RECV);

	setup_LED();
	setup_USB();
	setup_FPGA_comm();

	start_FPGA_comm();
	

	while(1) {
		if (buf_full) {
			if (buf_rdy) {
				buf_sel = (buf_sel) ? 0 : 1;
				buf_idx = 0;
				buf_full = false;
				buf_rdy = false;
			} else {
				stop_FPGA_comm();
			}
			/* TODO: Notify usbcontrol that a buffer is available for transfer submission */
		} else {
			if (buf_sel) {
				img_buf1[buf_idx] = buf_idx;
				set_LED(img_buf1[buf_idx] << 4);
			} else {
				img_buf0[buf_idx] = buf_idx;
				set_LED(img_buf0[buf_idx] << 4);
			}
		}
	}
}