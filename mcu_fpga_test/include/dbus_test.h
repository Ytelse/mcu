#ifndef __DBUS_TEST_H_
#define __DBUS_TEST_H_

/* Test function for displaying the lowest half-byte on the DBUS on the LEDs */
void display_bus_on_led_no_protocol(void);
/* Test function for displaying the lowest half-byte on the DBUS on the LEDs while conforming to the protocol */
void display_bus_on_led(void);
/* Test function for displying the lowest half-byte on the DBUS on the LEDs while conforming to the protocol and checking that the signal is valid before reading */
void display_bus_on_led_check_valid(void);

#endif
