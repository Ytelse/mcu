#include "dbus_test.h"

#include "dbus.h"
#include "leds.h"

void display_bus_on_led_no_protocol(void) {
	/* Get data */
	uint32_t data = DBUS_get_data();
	/* Display on LEDs */
	set_LED(data << 4);
	/* Tell FPGA to update values */
	DBUS_set_ACK();
	DBUS_clear_ACK();
}

void display_bus_on_led(void) {
	/* Assume that function is called from interrupt -> No VALID checking. */
	/* Get data */
	uint16_t data = DBUS_read();
	/* Display on LEDs */
	set_LED(data << 4);
}

void display_bus_on_led_check_valid(void) {
	while (!DBUS_get_VALID()) {/* Wait for DBUS signals to become valid before proceeding. */}
	uint16_t data = DBUS_read();
	set_LED(data << 4);
}