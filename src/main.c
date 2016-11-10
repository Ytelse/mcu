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
#define NUMBER_OF_IMAGES

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


  for(int i = 0; i < NUMBER_OF_PICTURES; i += 4) {
    // Set READY high
    GPIO_PinOutSet(E_BANK_PORT, PIN_READY);

    // Check the valid signal
    // FIX: Change to interrupt 
    while((int)GPIO_PinOutGet(E_BANK_PORT, PIN_VALID) == 0);

    int classification = -1;
    int bit0, bit1, bit2, bit3;

    // Read bits ...
    bit0 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA0);
    bit1 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA1);
    bit2 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA2);
    bit3 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA3);
    
    // And concatinate
    classification = bit0;
    classification |= (bit1 << 1);
    classification |= (bit2 << 2);
    classification |= (bit3 << 3);

    buffer[i] = classification;

    // Do it three more times for the other data lines
    classification = -1;
    bit0 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA4);
    bit1 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA5);
    bit2 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA6);
    bit3 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA7);
    classification = bit0;
    classification |= (bit1 << 1);
    classification |= (bit2 << 2);
    classification |= (bit3 << 3);
    buffer[i+1] = classification;

    classification = -1;
    bit0 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA8);
    bit1 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA9);
    bit2 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA10);
    bit3 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA11);
    classification = bit0;
    classification |= (bit1 << 1);
    classification |= (bit2 << 2);
    classification |= (bit3 << 3);
    buffer[i+2] = classification;

    classification = -1;
    bit0 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA12);
    bit1 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA13);
    bit2 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA14);
    bit3 = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA15);
    classification = bit0;
    classification |= (bit1 << 1);
    classification |= (bit2 << 2);
    classification |= (bit3 << 3);
    buffer[i+3] = classification;    
    
    /* buffer[i+0] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA0); */
    /* buffer[i+1] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA1); */
    /* buffer[i+2] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA2); */
    /* buffer[i+3] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA3); */

    /* buffer[i+4] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA4); */
    /* buffer[i+5] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA5); */
    /* buffer[i+6] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA6); */
    /* buffer[i+7] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA7); */

    /* buffer[i+8] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA8); */
    /* buffer[i+9] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA9); */
    /* buffer[i+10] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA10); */
    /* buffer[i+11] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA11); */

    /* buffer[i+12] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA12); */
    /* buffer[i+13] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA13); */
    /* buffer[i+14] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA14); */
    /* buffer[i+15] = (int)GPIO_PinOutGet(E_BANK_PORT, PIN_DATA15); */

    // Set ACK high
    GPIO_PinOutSet(E_BANK_PORT, PIN_ACK);
    // Wait some cycles
    sleep(1000);
    // Set ACK low
    GPIO_PinOutClear(E_BANK_PORT, PIN_ACK);
    
    // Repeat
  }
  
  
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


