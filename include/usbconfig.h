#ifndef __USBCONFIG_H_
#define __USBCONFIG_H_

/* Compile USB stack for device mode */
#define USB_DEVICE

/* Number of endpoints used */
#define NUM_EP_USED 2

/* 
	Let the USB stack use TIMER0.
	NOTE: This timer must not be used by the application.
*/
#define USB_TIMER USB_TIMER0

/* Define endpoint adresses */
#define EP_IN  0x81 /* Device to host */
#define EP_OUT 0x01 /* Host to device */

/* No power saving */
#define USB_PWRSAVE_MODE USB_PWRSAVE_MODE_OFF

#endif /* _USBCONFIG_H_ */