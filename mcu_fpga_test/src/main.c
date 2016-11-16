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

/* BUFFER STATUS VARIABLES */
volatile unsigned int 	buf_idx; 	/* Index of the first unwritten memory location of active buffer */
volatile uint8_t 		buf_sel; 	/* Denotes which buffer is currently active, 0 -> buf0, 1 -> buf1 */
volatile bool 			buf_full; 	/* Denotes whether active buffer is full or not */
volatile bool			buf_rdy;	/* Denotes whether inactive buffer is finished being written over USB */

volatile bool _wait, _halt;

volatile uint32_t msTicks;

void SysTick_Handler(void) {
	msTicks++;       /* increment counter necessary in Delay()*/
}

void Delay(uint32_t dlyTicks) {
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) ;
}

void switch_buf(void) {
	buf_sel = (buf_sel) ? 0 : 1; 	/* Select the buffer that is not in use */
	buf_idx = 0; 					/* Reset buf_idx */
	buf_full = false;				/* New buffer is not full */
	buf_rdy = false;				/* Inactive buffer is not ready */

	resume_FPGA_comm();				/* We have a active buffer, so communiction with FPGA can resume */
}

void setup_ctrl(void) {
	buf_idx = 0;
	buf_sel = 0;
	buf_full = false;
	buf_rdy = true;
	_wait = true;
	_halt = false;
}

int main(void) {
	CHIP_Init();

	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

	/* Set up control variables */

	setup_ctrl();

	/* Initialize image buffers to 0 */
	memset(img_buf0, 0, BUFFERSIZE_SEND);
	memset(img_buf1, 0, BUFFERSIZE_SEND);

	/* Setup LED pins */
	setup_LED();
	/* Setup USB stack */
	setup_USB();
	/* Setup FPGA communication I/O pins */
	setup_FPGA_comm();

	while (_wait) { 
		/* Wait for ready signal from USB host */
		/* In test mode, output data bus on LEDs */
		display_bus_on_led();
		//Delay(1);
		//set_ack_low();
		Delay(50);
	};

	/* Start communcation with FPGA */
	start_FPGA_comm();

	while(!_halt) {
		if (buf_full && buf_rdy) {
			switch_buf();
			notify_img_buf_ready();
		} else {
			//set_LED(buf_idx << 4);
		}
	}
	stop_FPGA_comm();
	set_LED(LEDS_ALL_ON);
}