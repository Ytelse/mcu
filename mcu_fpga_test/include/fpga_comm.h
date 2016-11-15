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


#endif /* __FPGA_COMM_H_ */