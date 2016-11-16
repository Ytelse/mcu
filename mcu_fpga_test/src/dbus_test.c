#include "dbus_test.h"

#include "dbus.h"
#include "leds.h"

void display_bus_on_led_no_protocol(void) {
	LEDS_update_all(DBUS_get_data() << 4);
	/* Tell FPGA to update values */
	DBUS_set_ACK();
	DBUS_clear_ACK();
}

void display_bus_on_led(void) {
	/* Assume that function is called from interrupt -> No VALID checking. */
	LEDS_update_all(DBUS_read() << 4);
}

void display_bus_on_led_check_valid(void) {
	/* Wait for DBUS signals to become valid before proceeding. */
	while (!DBUS_get_VALID()) {
		/* Do nothing */
	}
	LEDS_update_all(DBUS_read() << 4);
}