#include "em_gpio.h"
#include "em_cmu.h"
#include "gpio_control.h"

const int E_BANK_PORT =  0x0;
const int E_BANK_PIN = 0x0;

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
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA0, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA1, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA2, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA3, gpioModePushPull, 0);

  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA4, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA5, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA6, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA7, gpioModePushPull, 0);

  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA8, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA9, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA10, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA11, gpioModePushPull, 0);

  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA12, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA13, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA14, gpioModePushPull, 0);
  GPIO_PinModeSet(E_BANK_PORT, PIN_DATA15, gpioModePushPull, 0);

}
