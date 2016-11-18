#ifndef __CALLBACKS_H_
#define __CALLBACKS_H_

int  setupCmd(const USB_Setup_TypeDef *setup);
void stateChange(USBD_State_TypeDef oldState, USBD_State_TypeDef newState);
int  dataReceivedCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
int  dataSentCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

#endif