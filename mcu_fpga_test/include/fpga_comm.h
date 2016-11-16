#ifndef __FPGA_COMM_H_
#define __FPGA_COMM_H_

/* TODO: Remove this file, no longer needed after new implementation of DBUS */

#include "em_device.h"
#include "em_gpio.h"
#include "em_bus.h"

/* CONTROL LINES */

#define DBUS_CTRL_PORT 		2 /* gpioPortC */
#define DBUS_CTRL_PIN_RDY 	6 /* Pin 6 */
#define DBUS_CTRL_PIN_VALID 7 /* Pin 7 */
#define DBUS_CTRL_PIN_ACK  	8 /* Pin 8 */

#define DBUS_DATA_PORT 		4 /* gpioPortE */

/* TODO: Validate bit order */

/* HALF-BYTE 0 */
#define DBUS_DATA_PIN0 		0 	/* LSB */
#define DBUS_DATA_PIN1 		1
#define DBUS_DATA_PIN2 		2
#define DBUS_DATA_PIN3 		3 	/* MSB */
/* HALF-BYTE 1 */
#define DBUS_DATA_PIN4 		4 	/* LSB */
#define DBUS_DATA_PIN5 		5
#define DBUS_DATA_PIN6		6
#define DBUS_DATA_PIN7		7 	/* MSB */
/* HALF-BYTE 2 */
#define DBUS_DATA_PIN8		8 	/* LSB */
#define DBUS_DATA_PIN9		9
#define DBUS_DATA_PIN10		10
#define DBUS_DATA_PIN11		11 	/* MSB */
/* HALF-BYTE 3 */
#define DBUS_DATA_PIN12		12
#define DBUS_DATA_PIN13		13
#define DBUS_DATA_PIN14		14
#define DBUS_DATA_PIN15		15 	/* MSB */

void setup_FPGA_comm();

void start_FPGA_comm();
void stop_FPGA_comm();
void resume_FPGA_comm();
/* Test function */
uint32_t display_bus_on_led(void);
void set_ack_low(void);

#endif /* __FPGA_COMM_H_ */