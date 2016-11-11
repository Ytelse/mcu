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
#include "utils.h"

#define BUFFER_SIZE 64
#define NUMBER_OF_IMAGES 1024

state_t state;
int* buffer;

int VALID = 0;

int main(void)
{
  /* Chip errata */
  CHIP_Init();
  
  /* Enable HFXO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

  // This line is needed for Delay to work
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;


  /* setupEBI(); */
  /* setupLED(); */
  setupUSB();

  setupGPIO();

  // REMOVE FROM HERE ---------------
  GPIO_PinModeSet(4, 2, gpioModePushPull, 0);
  GPIO_PinOutClear(4, 2);
  GPIO_PinOutSet(4,2);
  Delay(1000);
  GPIO_PinOutClear(4, 2);
  // ---------------- TO HERE

  /* Buffer which is used to send and receive data */
  buffer = (int*)malloc(NUMBER_OF_IMAGES * sizeof(int));

  init_state();
    
  while(state.mcu_state != FINALIZE){
    switch(state.mcu_state) {
      case IDLE:
	while(state.mcu_state == IDLE) {
	  mcu_chill();
	}
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
  state.mcu_state = RUN; // Should be IDLE in production
  state.run_state = READY;
}

void mcu_chill() {
  if ( USBD_SafeToEnterEM2() ) {
    /* Enter EM2 when in suspend or disconnected */
    EMU_EnterEM2(true);
  }
  else {
    /* When USB is active we can sleep in EM1. */
    EMU_EnterEM1();
  }
}

void mcu_run_loop() {
  /* Recieve data from FPGA and send it to PC when transfer is done */
  for(int i = 0; i < NUMBER_OF_IMAGES; i += 4) {
    // Set READY high
    GPIO_PinOutSet(PIN_READY.port, PIN_READY.pin);

    // Check the valid signal
    while(VALID == 0) {
      mcu_chill();
    }
    VALID = 0;

    // One classification for every 4 bits
    int classification = -1;
    int bit0, bit1, bit2, bit3;

    for (int j = 0; j < 4; j++) {
      // Read bits ...
      bit0 = (int)GPIO_PinOutGet(PIN_DATA_ARRAY[0 + j*4].port, PIN_DATA_ARRAY[0 + j*4].pin);
      bit1 = (int)GPIO_PinOutGet(PIN_DATA_ARRAY[1 + j*4].port, PIN_DATA_ARRAY[1 + j*4].pin);
      bit2 = (int)GPIO_PinOutGet(PIN_DATA_ARRAY[2 + j*4].port, PIN_DATA_ARRAY[2 + j*4].pin);
      bit3 = (int)GPIO_PinOutGet(PIN_DATA_ARRAY[3 + j*4].port, PIN_DATA_ARRAY[3 + j*4].pin);
    
      // And concatinate
      classification = bit0;
      classification |= (bit1 << 1);
      classification |= (bit2 << 2);
      classification |= (bit3 << 3);

      buffer[i+j] = classification;
      
    }
    
    // Set ACK high
    GPIO_PinOutSet(PIN_ACK.port, PIN_ACK.pin);
    GPIO_PinOutSet(4,2);

    // Wait some cycles
    Delay(50);
    
    // Set ACK low
    GPIO_PinOutClear(PIN_ACK.port, PIN_ACK.pin);
    GPIO_PinOutClear(4,2);
    // REMOVE FROM HERE  --------------
    Delay(50);
    // ---------------- TO HERE
    
    // Repeat
  }

  // Transfer back to PC
  USBD_Write(EP_IN, buffer, sizeof(buffer), dataSentCallback);

  // REMOVE FROM HERE -------------
  Delay(1000);
  GPIO_PinModeSet(4, 3, gpioModePushPull, 0);
  GPIO_PinOutSet(4,3);
  // ---------------- TO HERE
  
  // Finished
  state.mcu_state = IDLE; // Finalize or IDLE?
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
