#include "em_ebi.h"
#include "em_cmu.h"

int setupEBI(void) {
   /* ------------------------------------------ */
  /* NAND Flash, Bank0, Base Address 0x80000000 */
  /* Micron flash NAND256W3A                    */
  /* ------------------------------------------ */

  EBI_Init_TypeDef ebiConfig =
  {   ebiModeD8A8,       /* 8 bit address, 8 bit data */
      ebiActiveLow,      /* ARDY polarity */
      ebiActiveLow,      /* ALE polarity */
      ebiActiveLow,      /* WE polarity */
      ebiActiveLow,      /* RE polarity */
      ebiActiveLow,      /* CS polarity */
      ebiActiveLow,      /* BL polarity */
      false,             /* disble BL */
      true,              /* enable NOIDLE */
      false,             /* disable ARDY */
      true,              /* disable ARDY timeout */
      EBI_BANK0,         /* enable bank 0 */
      0,                 /* no chip select */
      0,                 /* addr setup cycles */
      0,                 /* addr hold cycles */
      false,             /* disable half cycle ALE strobe */
      0,                 /* read setup cycles */
      2,                 /* read strobe cycles */
      1,                 /* read hold cycles */
      false,             /* disable page mode */
      false,             /* disable prefetch */
      false,             /* disable half cycle REn strobe */
      0,                 /* write setup cycles */
      2,                 /* write strobe cycles */
      1,                 /* write hold cycles */
      false,             /* enable the write buffer */
      false,             /* disable half cycle WEn strobe */
      ebiALowA24,        /* ALB - Low bound, address lines */
      ebiAHighA26,       /* APEN - High bound, address lines */
      ebiLocation1,      /* Use Location 1 */
      true,              /* enable EBI */
  };

  /* Enable clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_EBI, true);

  /* Enable GPIO's */
  /* ALE and CLE */
  /* GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 0); */

  /* /\* WP, CE and R/B *\/ */
  /* GPIO_PinModeSet(gpioPortD, 13, gpioModePushPull, 0);   /\* active low write-protect *\/ */
  /* GPIO_PinModeSet(gpioPortD, 14, gpioModePushPull, 1);   /\* active low chip-enable *\/ */
  /* GPIO_PinModeSet(gpioPortD, 15, gpioModeInput, 0);      /\* ready/busy *\/ */

  /* /\* IO pins *\/ */
  /* GPIO_PinModeSet(gpioPortE, 8, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 9, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 0); */
  /* GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 0); */

  /* /\* WE and RE *\/ */
  /* GPIO_PinModeSet(gpioPortF, 8, gpioModePushPull, 1); */
  /* GPIO_PinModeSet(gpioPortF, 9, gpioModePushPull, 1); */

  /* /\* NAND Power Enable *\/ */
  /* GPIO_PinModeSet(gpioPortB, 15, gpioModePushPull, 1); */

  EBI_Init(&ebiConfig);
  /* EBI->NANDCTRL = (EBI_NANDCTRL_BANKSEL_BANK0 | EBI_NANDCTRL_EN); */
  // ish?
  
  return 0;
}
