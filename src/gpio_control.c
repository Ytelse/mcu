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

}
