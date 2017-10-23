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


/*==================================================*/
   void configure_clocks();

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	timer_a0_config();
	ADC_CONFIG();
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


void configure_clocks(void){
    CS->KEY = 0x695A; //unlock 5.3.1
    CS->CTL0 = 0 ;
    CS->CTL0 |= CS_CTL0_DCOEN | CS_CTL0_DCORSEL_2;  //enable DCO, then mode 3 for 8 to 16 Mhz
    //CS->CTL0 |=0b1100111000; //and should be 4MHZ, got 4.5 MHZ, -200.5?
    CS->CTL0 |= 0b1011010100; //goal: 4MHZ, got:  4.016MHZ          @-300
   //   CS->CTL0 |= 0b1011010001; //DCTO-303 got: 3.88Mhz
    //CS->CTL0 |= 0b0100000110;


        //we want the baud rate clock to be 4 Mhz

    /*          select clock sources:
     * SELA => ACLK source = reforclock
     * SELS => SMCLK and HSMCLK  = DCO //p 4.4 secondary mode!!
     * SELM => MCLK Source = DCO
     */

    //lets tune the DCO clock with the right frequency. goal is 4MhZ
   // uint32_t myConstant_K = TLV->DCOER_CONSTK_RSEL04; //constant for K_Constant for equation on page 312
    //uint32_t myConstant_FCAL = TLV->DCOIR_FCAL_RSEL04; // constant for F_CAL for equation


    //not a typo definitely needs to be CLT1 and not CTL0
    //lets divide the SMCLK clock by some divisor to get the create the frequency
    CS->CTL1 |= CS_CTL1_SELA__REFOCLK | CS_CTL1_SELS__DCOCLK | CS_CTL1_SELM__DCOCLK;



    CS->KEY = 0; //lock
}
//:TODO UART requires 4Mhz Clock so need to be sure to account for that
        // I am going to use the 4Mhz clock value for my CCR[0] value calculation
