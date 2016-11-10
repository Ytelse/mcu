#ifndef _GPIO_H
#define _GPIO_H

void setupGPIO(void);
void gpio_init(void);

extern const int E_BANK_PORT;
extern const int E_BANK_PIN;

extern const int PIN_READY;
extern const int PIN_VALID;
extern const int PIN_ACK;

extern const int PIN_DATA0;
extern const int PIN_DATA1;
extern const int PIN_DATA2;
extern const int PIN_DATA3;

#endif
