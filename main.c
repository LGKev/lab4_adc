#include "msp.h"
#include "adc_driver.h"
#include "lab4.h"
#include "timer.h"

/*
 * main.c
 */

/*==================================================*/
/*     Globals                                      */

   volatile uint8_t FLAG_Collect_Data = 0; //collect data from the mem[0] when set to 1


/*==================================================*/

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	timer_a0_config();
	ADC_CONFIG();

	P2SEL0 &= ~BIT2; //gpio
	P2SEL1 &= ~BIT2; //gpio
	P2->DIR = BIT2;
	P2->OUT = BIT2;

	__enable_interrupt();
	while(1){
	    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC; //start sample, enable conversion?

	    if(FLAG_Collect_Data == 1){
	        //add to buffer but for now just put into variable
	        uint16_t data = ADC14->MEM[0]; //get data stored in memory 0.
	        //:Todo add to buffe
	        FLAG_Collect_Data = 0;
	    }
	}
}


//:TODO UART requires 4Mhz Clock so need to be sure to account for that
        // I am going to use the 4Mhz clock value for my CCR[0] value calculation
