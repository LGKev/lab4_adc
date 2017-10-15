#include "msp.h"
#include "adc_driver.h"
#include "lab4.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

#ifdef INTERNAL_TEMPERATURE_READ
	ADC_CONFIG();
#endif

	__enable_interrupt();

	while(1){
	    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC; //start sample, enable conversion?
	}
}
