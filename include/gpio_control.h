#ifndef _GPIO_H
#define _GPIO_H

typedef struct {
  GPIO_Port_TypeDef port;
  unsigned int      pin;
} port_pin_t;

void setupGPIO(void);
void gpio_init(void);
void GPIO_ODD_IRQHandler(void);


extern const port_pin_t PIN_READY;
extern const port_pin_t PIN_VALID;
extern const port_pin_t PIN_ACK;

extern const port_pin_t PIN_DATA0;
extern const port_pin_t PIN_DATA1;
extern const port_pin_t PIN_DATA2;
extern const port_pin_t PIN_DATA3;

extern const port_pin_t PIN_DATA4;
extern const port_pin_t PIN_DATA5;
extern const port_pin_t PIN_DATA6;
extern const port_pin_t PIN_DATA7;
extern const port_pin_t PIN_DATA8;
extern const port_pin_t PIN_DATA9;
extern const port_pin_t PIN_DATA10;
extern const port_pin_t PIN_DATA11;
extern const port_pin_t PIN_DATA12;
extern const port_pin_t PIN_DATA13;
extern const port_pin_t PIN_DATA14;
extern const port_pin_t PIN_DATA15;

extern port_pin_t PIN_DATA_ARRAY[16];

#endif
