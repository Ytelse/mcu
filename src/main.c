#include <string.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"

#include "usb_control.h"
#include "ebi_control.h"
#include "led_control.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();
  
  /* Enable HFXO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  
  setupEBI();
  setupLED();
  setupUSB();
    
  while(1) { /* Do nothing */ } 
}
