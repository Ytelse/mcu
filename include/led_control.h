#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

#ifndef STK
#define LED_PORT gpioPortD
#define LED_PIN0 4
#define LED_PIN1 5
#define LED_PIN2 6
#define LED_PIN3 7

int setupLED(void);
int setLEDs(uint32_t mask);
int toggleLEDs(uint32_t mask);

#endif

#endif

