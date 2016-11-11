#ifndef _GPIO_H
#define _GPIO_H

void setupGPIO(void);
void gpio_init(void);
void GPIO_ODD_IRQHandler(void);

extern const int E_BANK_PORT;
extern const int E_BANK_PIN;

extern const int PIN_READY;
extern const int PIN_VALID;
extern const int PIN_ACK;

extern const int PIN_DATA0;
extern const int PIN_DATA1;
extern const int PIN_DATA2;
extern const int PIN_DATA3;

extern const int PIN_DATA4;
extern const int PIN_DATA5;
extern const int PIN_DATA6;
extern const int PIN_DATA7;
extern const int PIN_DATA8;
extern const int PIN_DATA9;
extern const int PIN_DATA10;
extern const int PIN_DATA11;
extern const int PIN_DATA12;
extern const int PIN_DATA13;
extern const int PIN_DATA14;
extern const int PIN_DATA15;

extern int PIN_DATA_ARRAY[16];

#endif
