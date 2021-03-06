#include "msp.h"
#include "adc_driver.h"
#include "lab4.h"
#include "timer.h"
#include "uart.h"
#include"circ_buffer_basic.h"

/*
 * main.c
 */

/*==================================================*/
/*     Globals                                      */

   volatile uint8_t FLAG_Collect_Data = 0; //collect data from the mem[0] when set to 1
   volatile uint8_t Calculate_Stats = 0; // calculate stats on datas :Todo remove this variable
   CircBuf_t myBuffer;
   CircBuf_t * myBufferPTR = &myBuffer;

   //:Accelerometer
   volatile uint16_t X_ADC = 0;
   volatile uint16_t Y_ADC = 0;
   volatile uint16_t Z_ADC = 0;

   //:Joystick

   //Temperature
   volatile uint16_t NADC_Temperature = 0;
/*==================================================*/

void main(void)
{

    initialize_Circ_Buffer(&myBufferPTR, 60);

    //enable the button interrupt
    Button_Interrupt_Config();

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	timer_a0_config();
	ADC_CONFIG(); //generic ADC, for channel 0.
	//ADC_CONFIG_Accelerometer();
	//ADC_CONFIG_Joystick();
	UART_config();
	configure_clocks();

	buzzer_Config();
	buffer_Full_Config();

	/*visual output*/
	P2SEL0 &= ~BIT2; //gpio
	P2SEL1 &= ~BIT2; //gpio
	P2->DIR = BIT2;
	P2->OUT = BIT2;

	P1SEL0 &= ~BIT0;
	P1SEL1 &= ~BIT0;
	P1->DIR = BIT0;
	P1->OUT &= ~BIT0;



	__enable_interrupt();
	while(1){



	    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC; //start sample, enable conversion?

	    if(FLAG_Collect_Data == 1){
	        //add to buffer but for now just put into variable
	        //:Todo add to buffer
	        __disable_interrupts();
	        add_To_Buffer(&myBufferPTR, NADC_Temperature);


	        float  dataRead_float_C =  Temperature_from_voltage_Celsius(NADC_Temperature, 1.2, 14);
	        char dataString_C[10];
	        ftoa(dataRead_float_C, dataString_C, 3);


	        UART_putchar_n(dataString_C, strlen(dataString_C));
	        //add units: C
	        UART_putchar_n(" �C  ", strlen(" �C  "));
	        UART_putchar(13);

	        __enable_interrupts();
//	        UART_putchar_n("test", 4);
//	        UART_putchar(13);
	        FLAG_Collect_Data = 0;
	    }
	}
}


