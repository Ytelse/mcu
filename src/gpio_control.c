#include "em_gpio.h"
#include "em_cmu.h"
#include "gpio_control.h"
#include "main.h"


/* Non interfering pins for the STK */
/* const port_pin_t PIN_READY = (port_pin_t){.port = gpioPortA, .pin = 2};  // D0 */
/* const port_pin_t PIN_VALID = (port_pin_t){.port = gpioPortA, .pin = 3};  // D1 */
/* const port_pin_t PIN_ACK   = (port_pin_t){.port = gpioPortA, .pin = 4}; // D2 */

/* port_pin_t PIN_DATA_ARRAY[16]; */

/* const port_pin_t PIN_DATA0 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A0 */
/* const port_pin_t PIN_DATA1 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A1 */
/* const port_pin_t PIN_DATA2 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A2 */
/* const port_pin_t PIN_DATA3 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A3 */

/* const port_pin_t PIN_DATA4 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A4 */
/* const port_pin_t PIN_DATA5 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A5 */
/* const port_pin_t PIN_DATA6 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A6 */
/* const port_pin_t PIN_DATA7 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A7 */

/* const port_pin_t PIN_DATA8 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A8 */
/* const port_pin_t PIN_DATA9 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A9 */
/* const port_pin_t PIN_DATA10 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A10 */
/* const port_pin_t PIN_DATA11 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A11 */

/* const port_pin_t PIN_DATA12 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A12 */
/* const port_pin_t PIN_DATA13 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A13 */
/* const port_pin_t PIN_DATA14 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A14 */
/* const port_pin_t PIN_DATA15 = (port_pin_t){.port = gpioPortE, .pin = 1}; // A15 */

const port_pin_t PIN_READY = (port_pin_t){.port = gpioPortE, .pin = 8};  // D0
const port_pin_t PIN_VALID = (port_pin_t){.port = gpioPortE, .pin = 9};  // D1
const port_pin_t PIN_ACK   = (port_pin_t){.port = gpioPortE, .pin = 10}; // D2

port_pin_t PIN_DATA_ARRAY[16];

const port_pin_t PIN_DATA0 = (port_pin_t){.port = gpioPortA, .pin = 12}; // A0
const port_pin_t PIN_DATA1 = (port_pin_t){.port = gpioPortA, .pin = 13}; // A1
const port_pin_t PIN_DATA2 = (port_pin_t){.port = gpioPortA, .pin = 14}; // A2
const port_pin_t PIN_DATA3 = (port_pin_t){.port = gpioPortB, .pin = 9};  // A3

const port_pin_t PIN_DATA4 = (port_pin_t){.port = gpioPortB, .pin = 10}; // A4
const port_pin_t PIN_DATA5 = (port_pin_t){.port = gpioPortC, .pin = 6};  // A5
const port_pin_t PIN_DATA6 = (port_pin_t){.port = gpioPortC, .pin = 7};  // A6
const port_pin_t PIN_DATA7 = (port_pin_t){.port = gpioPortE, .pin = 0};  // A7

const port_pin_t PIN_DATA8 = (port_pin_t){.port = gpioPortE, .pin = 1};  // A8
const port_pin_t PIN_DATA9 = (port_pin_t){.port = gpioPortE, .pin = 2};  // A9
const port_pin_t PIN_DATA10 = (port_pin_t){.port = gpioPortE, .pin = 3}; // A10
const port_pin_t PIN_DATA11 = (port_pin_t){.port = gpioPortE, .pin = 4}; // A11

const port_pin_t PIN_DATA12 = (port_pin_t){.port = gpioPortE, .pin = 5}; // A12
const port_pin_t PIN_DATA13 = (port_pin_t){.port = gpioPortE, .pin = 6}; // A13
const port_pin_t PIN_DATA14 = (port_pin_t){.port = gpioPortE, .pin = 7}; // A14
const port_pin_t PIN_DATA15 = (port_pin_t){.port = gpioPortC, .pin = 8}; // A15


void setupGPIO() {
  gpio_init();
}


void gpio_init() {

  /* Enable clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  // REMOVE FROM HERE ----------------------
  /* Enable interrupt on push button 0 */
  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
  GPIO_IntConfig(gpioPortB, 9, false, true, true);
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  // --------------- TO HERE
  
  /* READY */
  GPIO_PinModeSet(PIN_READY.port, PIN_READY.pin, gpioModePushPull, 0);
  
  /* VALID */
  GPIO_PinModeSet(PIN_VALID.port, PIN_VALID.pin, gpioModeInput, 0);
  /* Valid should be an interrupt signal */
  GPIO_IntConfig(PIN_VALID.port, PIN_VALID.pin,
		 true, /* risingEdge */
  		 false, /* fallingEdge */
  		 true);  /* enable */
  
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  
  /* ACK */
  GPIO_PinModeSet(PIN_ACK.port, PIN_ACK.pin, gpioModePushPull, 0);
  
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
    GPIO_PinModeSet(PIN_DATA_ARRAY[i].port, PIN_DATA_ARRAY[i].pin, gpioModePushPull, 0);
  }

}

void GPIO_ODD_IRQHandler(void)
{
  /* Clear the interrupt flag */
  GPIO->IFC = 1 << 9;

  // The interrupt indicates that the MCU can start reading data
  VALID = 1;
}
