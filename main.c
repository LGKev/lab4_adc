#include "msp.h"
#include "adc_driver.h"
#include "lab4.h"
#include "timer.h"
#include "Circular_Buffer_8_bit.h"
#include "uart.h"

/*
 * main.c
 */

/*==================================================*/
/*     Globals                                      */

   volatile uint8_t FLAG_Collect_Data = 0; //collect data from the mem[0] when set to 1
   volatile uint8_t Calculate_Stats = 0; // calculate stats on datas :Todo remove this variable
   CircBuf_t myBuffer;
   CircBuf_t * myBufferPTR = &myBuffer;
   volatile uint16_t X_ADC = 0;
   volatile uint16_t Y_ADC = 0;
   volatile uint16_t Z_ADC = 0;

/*==================================================*/

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	timer_a0_config();
	//ADC_CONFIG(); //generic ADC, for channel 0.
	ADC_CONFIG_Accelerometer();
	UART_config();
	configure_clocks();

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
	        //:Todo add to buffer


	        UART_putchar_n("test", 4);
	        UART_putchar(13);
	        FLAG_Collect_Data = 0;
	    }
	}
}


