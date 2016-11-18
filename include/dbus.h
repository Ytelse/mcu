#ifndef __DBUS_H_
#define __DBUS_H_

#include "em_device.h"
#include "em_bus.h"


/* CONTROL LINES */

#define DBUS_RDY_PORT 		0 		/* GPIO port A 	*/
#define DBUS_RDY_PIN 		15		/* Pin 15		*/

#define DBUS_ACK_PORT 		2 		/* GPIO port C 	*/
#define DBUS_ACK_PIN 		8		/* Pin 8		*/

#define DBUS_VLD_PORT 		2 		/* GPIO port C 	*/
#define DBUS_VLD_PIN 		7		/* Pin 7		*/

/* DATA LINES */

#define DBUS_DATA_PORT 		4 		/* GPIO port E */

/* HALF-BYTE 0 */
#define DBUS_DATA_PIN0 		0 		/* LSB */ /* Pin 0 */
#define DBUS_DATA_PIN1 		1
#define DBUS_DATA_PIN2 		2
#define DBUS_DATA_PIN3 		3 		/* MSB */ /* Pin 3 */

/* HALF-BYTE 1 */
#define DBUS_DATA_PIN4 		4 		/* LSB */ /* Pin 4 */
#define DBUS_DATA_PIN5 		5
#define DBUS_DATA_PIN6		6
#define DBUS_DATA_PIN7		7 		/* MSB */ /* Pin 7 */

/* HALF-BYTE 2 */
#define DBUS_DATA_PIN8		8 		/* LSB */ /* Pin 8 */
#define DBUS_DATA_PIN9		9
#define DBUS_DATA_PIN10		10
#define DBUS_DATA_PIN11		11 		/* MSB */ /* Pin 1 */

/* HALF-BYTE 3 */
#define DBUS_DATA_PIN12		12		/* LSB */ /* Pin 12 */
#define DBUS_DATA_PIN13		13
#define DBUS_DATA_PIN14		14
#define DBUS_DATA_PIN15		15 		/* MSB */ /* Pin 15 */

/* Function prototype for setting up the necessary GPIO pins */

void DBUS_init(void);

/**
 * Inlined version of a DBUS read.
 * Gets the data currently registered on the DBUS (GPIO port E).
 */

__STATIC_INLINE uint32_t DBUS_get_data(void) {
	return GPIO->P[DBUS_DATA_PORT].DIN;
}

/**
 * Inlined version of a ACK pin set.
 * Sets ACK high (logical true).
 */

__STATIC_INLINE void DBUS_set_ACK(void) {
	#ifdef _GPIO_P_DOUTSET_MASK
	GPIO->P[DBUS_ACK_PORT].DOUTSET = 1 << DBUS_ACK_PIN;
	#else
	BUS_RegBitWrite(&GPIO->P[DBUS_ACK_PORT].DOUT, DBUS_ACK_PIN, 1);
	#endif
}

/**
 * Inlined version of a ACK pin clear.
 * Sets ACK low (logical false).
 */

__STATIC_INLINE void DBUS_clear_ACK(void) {
	#ifdef _GPIO_P_DOUTCLR_MASK
	GPIO->P[DBUS_ACK_PORT].DOUTCLR = 1 << DBUS_ACK_PIN;
	#else
	BUS_RegBitWrite(&GPIO->P[DBUS_ACK_PORT].DOUT, DBUS_ACK_PIN, 0);
	#endif
}

/**
 * Inlined version of a READY pin set.
 * Sets READY high (logical true).
 */

__STATIC_INLINE void DBUS_set_READY(void) {
	#ifdef _GPIO_P_DOUTSET_MASK
	GPIO->P[DBUS_RDY_PORT].DOUTSET = 1 << DBUS_RDY_PIN;
	#else
	BUS_RegBitWrite(&GPIO->P[DBUS_RDY_PORT].DOUT, DBUS_RDY_PIN, 1);
	#endif
}

/**
 * Inlined version of a READY pin clear.
 * Sets READY low (logical false).
 */

__STATIC_INLINE void DBUS_clear_READY(void) {
	#ifdef _GPIO_P_DOUTCLR_MASK
	GPIO->P[DBUS_RDY_PORT].DOUTCLR = 1 << DBUS_RDY_PIN;
	#else
	BUS_RegBitWrite(&GPIO->P[DBUS_RDY_PORT].DOUT, DBUS_RDY_PIN, 0);
	#endif
}

/**
 * Inlined version of a VALID pin read.
 * Returns the value of VALID.
 */

__STATIC_INLINE unsigned int DBUS_get_VALID(void) {
	return BUS_RegBitRead(&GPIO->P[DBUS_VLD_PORT].DIN, DBUS_VLD_PIN);
}

/** 
 * Attempt at creating a inlined DBUS read that conforms to our 
 * communication protocol.
 *
 * Function reads the DBUS and returns the values on the data bus
 * as a 16 bit unsigned int.
 *
 * NOTE: Caller must ensure that the data on the bus is valid before
 * issuing a read. This can be done either by polling DBUS_get_VALID()
 * or setting up a GPIO interrupt on DBUS_CTRL_PIN_VALID.
 */

__STATIC_INLINE __attribute__((optimize("O0"))) uint16_t DBUS_read(void) {
	DBUS_clear_READY();
	uint16_t data = (uint16_t) DBUS_get_data();
	DBUS_set_ACK();
	DBUS_set_READY();
	DBUS_clear_ACK();
	return data;
}

/* ======================================================== */
/* =           APPLICATION SPECIFIC FUNCTIONS             = */
/* ======================================================== */

/** 
 * Attempt at creating a inlined DBUS read that conforms to our 
 * communication protocol and stores the values in memory.
 *
 * Function reads the DBUS, splits the 16 bits into 2 bytes,
 * and stores the upper byte in the memory location pointed to by (ptr)
 * and the lower byte in (ptr + 1).
 *
 * NOTE: This function requires that the caller keeps track 
 * of buffer capacity, and does not call this function unless
 * ptr is a known memory location.
 *
 * NOTE: Caller must ensure that the data on the bus is valid before
 * issuing a read. This can be done either by polling DBUS_get_VALID()
 * or setting up a GPIO interrupt on DBUS_CTRL_PIN_VALID.
 *
 * EXAMPLE: read, increment idx and check address like this:
 * 				DBUS_read(&img_buf[idx]); idx+=2;
 *				if (idx == BUFFERSIZE) {-- switch buffers --}
 */

__STATIC_INLINE void DBUS_read_and_store(uint8_t* ptr) {
	/* Set READY low */
	DBUS_clear_READY();
	/* Get DATA */
	uint16_t tmp_data = (uint16_t) DBUS_get_data();
	/* Set ACK high */
	DBUS_set_ACK();
	/* Store data in bufptr */
	*ptr++ = (uint8_t) (tmp_data >> 8); /* Upper byte */
	*ptr++ = (uint8_t) (tmp_data);		/* Lower byte */

	/* All done, set ACK low and READY high */
	DBUS_clear_ACK();
	DBUS_set_READY();
}

/* Function for letting FPGA know that we are ready to receive data */

void DBUS_start(void);

/**
 * Inlined function for pausing communication with FPGA. Necessary
 * during buffer switches, etc.
 */

__STATIC_INLINE void DBUS_pause(void) {
	DBUS_clear_READY();
}

/**
 * Inlined function for resuming communication with FPGA.
 */

__STATIC_INLINE void DBUS_resume(void) {
	DBUS_set_READY();
}

/* Function for completely stopping all communication with FPGA */

void DBUS_stop(void);

/* ======================================================== */
/* =                ROOM FOR TEST FUNCTIONS               = */
/* ======================================================== */


#endif /* __DBUS_H_ */