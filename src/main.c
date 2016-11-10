#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_usb.h"
#include "em_ebi.h"

#include "usb_control.h"
#include "ebi_control.h"
#include "led_control.h"
#include "usb_callbacks.h"
#include "gpio_control.h"
#include "main.h"

#define BUFFER_SIZE 64
#define NUMBER_OF_IMAGES 1024

state_t state;
int* buffer;

int main(void)
{
  /* Chip errata */
  CHIP_Init();
  
  /* Enable HFXO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  
  /* setupEBI(); */
  /* setupLED(); */
  /* setupUSB(); */

  setupGPIO();

  /* Buffer which is used to send and receive data */
  buffer = (int*)malloc(NUMBER_OF_IMAGES * sizeof(int));

  init_state();
    
  while(state.mcu_state != FINALIZE){
    switch(state.mcu_state) {
      case IDLE:
        mcu_chill();
        break;
      case RUN:
        // Get from FPGA and send to PC
        mcu_run_loop();
        break;
      case TESTRUN:
        mcu_test_run();
        // Create random values and send to PC
        break;
      case FINALIZE:
        // Clean up
        break;
    }
  }

  free(buffer);
  return 0;
}


void init_state() {
  state.mcu_state = IDLE;
  state.run_state = READY;
}

void mcu_chill() {
  while(state.mcu_state == IDLE) {
    if ( USBD_SafeToEnterEM2() ) {
	/* Enter EM2 when in suspend or disconnected */
	EMU_EnterEM2(true);
    } 
    else {
	/* When USB is active we can sleep in EM1. */
	EMU_EnterEM1();
    }
  }
}

void mcu_run_loop() {
  /* Recieve data from FPGA and send it to PC when transfer is done */

  for(int i = 0; i < NUMBER_OF_IMAGES; i += 4) {
    // Set READY high
    GPIO_PinOutSet(E_BANK_PORT, PIN_READY);

    // Check the valid signal
    // FIX: Change to interrupt 
    while((int)GPIO_PinOutGet(E_BANK_PORT, PIN_VALID) == 0);

    // One classification for every 4 bits
    int classification = -1;
    int bit0, bit1, bit2, bit3;

    for (int j = 0; j < 4; i++) {
      // Read bits ...
      bit0 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA_ARRAY[0 + j*4]);
      bit1 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA_ARRAY[1 + j*4]);
      bit2 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA_ARRAY[2 + j*4]);
      bit3 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA_ARRAY[3 + j*4]); 
    
      // And concatinate
      classification = bit0;
      classification |= (bit1 << 1);
      classification |= (bit2 << 2);
      classification |= (bit3 << 3);

      buffer[i] = classification;
      
    }
    
    // Set ACK high
    GPIO_PinOutSet(E_BANK_PORT, PIN_ACK);

    // Wait some cycles
    // sleep();
    
    // Set ACK low
    GPIO_PinOutClear(E_BANK_PORT, PIN_ACK);
    
    // Repeat
  }

  // Transfer back to PC
  
  // Finished
  state.mcu_state = IDLE;
}

void mcu_test_run() {
  /* Fill buffer with random numbers */
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = (int) rand();
  }

  /* Write to PC */
  USBD_Write(EP_IN, buffer, sizeof(buffer), dataSentCallback);

  // Finished
  state.mcu_state = IDLE;
}


// REFACTOR CODE AFTER HERE

/* #define BANK0_BASE_ADDR 0x80000000 */
/* #define BANK1_BASE_ADDR 0x84000000 */
/* #define TEST_SIZE 16 */

/* int ebi_read(int address) { */
/*   return *(volatile int*)(BANK1_BASE_ADDR + (address << 1)); */
/* } */

/* /\* Read data from EBI and output something on LEDS / send data to PC *\/ */
/* void test_fpga_connection() { */
/*   int data[16] = {0}; */

  
/*   for (int i = 0; i < TEST_SIZE; i++) { */
/*     data[i] = ebi_read(i); */
/*   } */

/*   // Blink LEDS? */
/*   USBD_Write(EP_IN, data, sizeof(data), dataSentCallback); */

/* } */


