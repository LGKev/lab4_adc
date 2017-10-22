#include "msp.h"
#include "adc_driver.h"
#include "lab4.h"
#include "core_cm4.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT      (0x00000002)
/**
 * main.c
 */

void main(void)

{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

#ifdef INTERNAL_TEMPERATURE_READ
#endif
    ADC_CONFIG();
	__enable_interrupt();


	while(1){
	    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC; //start sample, enable conversion?

	}
}

