#ifndef __FPGA_COMM_H_
#define __FPGA_COMM_H_

#define DATA_BUS_PORT gpioPortE

#include "em_gpio.h"

typedef struct {
  GPIO_Port_TypeDef port;
  unsigned int      pin;
} port_pin_t;

void setup_FPGA_comm();

void start_FPGA_comm();
void stop_FPGA_comm();
void resume_FPGA_comm();
/* Test function */
uint32_t display_bus_on_led(void);
void set_ack_low(void);


#endif /* __FPGA_COMM_H_ */