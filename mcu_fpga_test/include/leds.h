#ifndef __LEDS_H_
#define __LEDS_H_

/* TODO: Define certain LED masks to denote status */

#define LEDS_ALL_ON		0x000000F0
#define LEDS_ALL_OFF	0x00000000
/* These can be OR'd in order to set a certain pattern */
#define LED0_ON			0x00000080
#define LED1_ON			0x00000010
#define LED2_ON			0x00000020
#define LED3_ON			0x00000040

/* LED port and pins on the PCB */
#define LED_PORT gpioPortD
#define LED_PIN0 4
#define LED_PIN1 5
#define LED_PIN2 6
#define LED_PIN3 7

int setup_LED(void);
int set_LED(uint32_t mask);

#endif

