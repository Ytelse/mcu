/**************************************************************************//**
 * @brief RTC Interrupt Handler, clears the flag.
 *****************************************************************************/
void RTC_IRQHandler(void);

/**************************************************************************//**
 * @brief  Start LFRCO for RTC
 * Starts the low frequency RC oscillator (LFRCO) and routes it to the RTC
 *****************************************************************************/
void startLfxoForRtc(void);



/**************************************************************************//**
 * @brief  Setup RTC
 * On compare match with COMP0, clear counter and set interrupt
 *****************************************************************************/
void setupRtc(void);
