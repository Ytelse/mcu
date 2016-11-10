#include "em_gpio.h"
#include "em_cmu.h"
#include "gpio_control.h"

const int E_BANK_PORT =  0x0; // Set this to something correct

const int PIN_READY = 0;
const int PIN_VALID = 1;
const int PIN_ACK = 2;

const int PIN_DATA0 = 3;
const int PIN_DATA1 = 4;
const int PIN_DATA2 = 5;
const int PIN_DATA3 = 6;

const int PIN_DATA4 = 7;
const int PIN_DATA5 = 8;
const int PIN_DATA6 = 9;
const int PIN_DATA7 = 10;

const int PIN_DATA8 = 11;
const int PIN_DATA9 = 12;
const int PIN_DATA10 = 13;
const int PIN_DATA11 = 14;

const int PIN_DATA12 = 15;
const int PIN_DATA13 = 16;
const int PIN_DATA14 = 17;
const int PIN_DATA15 = 18;

int PIN_DATA_ARRAY[16];


void setupGPIO() {
  gpio_init();

}


void gpio_init() {

  /* Enable clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Set PIN modes GPIO_PinModeSet(port, pin, mode, out) */
  
  /* READY */
  GPIO_PinModeSet(E_BANK_PORT, PIN_READY, gpioModePushPull, 0);
  
  /* VALID */
  GPIO_PinModeSet(E_BANK_PORT, PIN_VALID, gpioModePushPull, 0);
  
  /* ACK */
  GPIO_PinModeSet(E_BANK_PORT, PIN_ACK, gpioModePushPull, 0);
  
  /* DATA */
  PIN_DATA_ARRAY[0] = PIN_DATA0; 
  PIN_DATA_ARRAY[1] = PIN_DATA1; 
  PIN_DATA_ARRAY[2] = PIN_DATA2; 
  PIN_DATA_ARRAY[3] = PIN_DATA3;
  PIN_DATA_ARRAY[4] = PIN_DATA4; 
  PIN_DATA_ARRAY[5] = PIN_DATA5; 
  PIN_DATA_ARRAY[6] = PIN_DATA6; 
  PIN_DATA_ARRAY[7] = PIN_DATA7;
  PIN_DATA_ARRAY[8] = PIN_DATA8; 
  PIN_DATA_ARRAY[9] = PIN_DATA9; 
  PIN_DATA_ARRAY[10] = PIN_DATA10; 
  PIN_DATA_ARRAY[11] = PIN_DATA11;
  PIN_DATA_ARRAY[12] = PIN_DATA12; 
  PIN_DATA_ARRAY[13] = PIN_DATA13; 
  PIN_DATA_ARRAY[14] = PIN_DATA14; 
  PIN_DATA_ARRAY[15] = PIN_DATA15;

  for (int i = 0; i < 16; i++) {
    GPIO_PinModeSet(E_BANK_PORT, PIN_DATA_ARRAY[i], gpioModePushPull, 0);
  }

}
