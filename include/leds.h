#ifndef __LEDS_H_
#define __LEDS_H_

#include "em_device.h"
#include "em_bus.h"

/* LED GPIO port */
#define LED_PORT 3	/* GPIO Port E */

/* LED GPIO pins */
#define LED_PIN0 4	/* GPIO Pin 4 */
#define LED_PIN1 5	/* GPIO Pin 5 */
#define LED_PIN2 6	/* GPIO Pin 6 */
#define LED_PIN3 7	/* GPIO Pin 7 */

/* Convenient LED masks. Can be OR'd in order to create patterns */
#define LEDS_ALL		0x000000F0
#define LEDS_NONE		0x00000000
#define LED0			0x00000080	/* LED12  on PCB */
#define LED1			0x00000010	/* LED9 on PCB */
#define LED2			0x00000020	/* LED10 on PCB */
#define LED3			0x00000040	/* LED11 on PCB */

/**
 * Inlined function for displaying the given mask on the LEDs
 */

__STATIC_INLINE void LEDS_update_all(uint32_t mask) {
	/* Clear all LEDs before setting in order to update all LEDs to new value */
	#if defined( _GPIO_P_DOUTSET_MASK ) && ( _GPIO_P_DOUTCLR_MASK )
	GPIO->P[LED_PORT].DOUTCLR = LEDS_ALL;
	GPIO->P[LED_PORT].DOUTSET = mask;
	#else
	BUS_RegMaskedClear(&GPIO->P[LED_PORT].DOUT, LEDS_ALL);
	BUS_RegMaskedSet(&GPIO->P[LED_PORT].DOUT, mask);
	#endif
}

__STATIC_INLINE void LEDS_set_all(void) {
	#ifdef _GPIO_P_DOUTSET_MASK
	GPIO->P[LED_PORT].DOUTSET = LEDS_ALL;
	#else
	BUS_RegMaskedSet(&GPIO->P[LED_PORT].DOUT, LEDS_ALL);
	#endif
}

__STATIC_INLINE void LEDS_clear_all(void) {
	#ifdef _GPIO_P_DOUTCLR_MASK
	GPIO->P[LED_PORT].DOUTCLR = LEDS_ALL;
	#else
	BUS_RegMaskedClear(&GPIO->P[LED_PORT].DOUT, LEDS_ALL);
	#endif
}

__STATIC_INLINE void LEDS_set(uint32_t mask) {
	#ifdef _GPIO_P_DOUTSET_MASK
	GPIO->P[LED_PORT].DOUTSET = mask;
	#else
	BUS_RegMaskedSet(&GPIO->P[LED_PORT].DOUT, mask);
	#endif
}

__STATIC_INLINE void LEDS_clear(uint32_t mask) {
	#ifdef _GPIO_P_DOUTCLR_MASK
	GPIO->P[LED_PORT].DOUTCLR = mask;
	#else
	BUS_RegMaskedClear(&GPIO->P[LED_PORT].DOUT, mask);
	#endif
}

/* Initializer */
void LEDS_init(void);

#endif

