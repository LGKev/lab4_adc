/*
 * lab4.c
 *
 *  Created on: Oct 11, 2017
 *      Author: kk
 */

#include "math.h"
#include "lab4.h"
#include "msp.h"
#include "circ_buffer_basic.h"
#include "uart.h"

extern  CircBuf_t * myBufferPTR;

float voltage_from_ADC(uint16_t nadc, float vref, uint8_t resolution_number_of_bits){

    float voltage = vref*((float)nadc/pow(2,resolution_number_of_bits));

    return voltage;
}



float Temperature_from_voltage_Celsius(uint16_t nadc, float vref, uint8_t resolution_number_of_bits){
    float voltage = vref*((float)nadc/(float)pow(2,resolution_number_of_bits)); //voltage in VOLTS

    float temperature_Celcius = voltage/(float)(0.019) + (float)0.685;

    return temperature_Celcius;
}




void Button_Interrupt_Config(){
        P1->SEL0 &= ~BIT1;    //
        P1->SEL1 &= ~BIT1;    //General I/O is enable for P1.1
        P1->OUT |= BIT1;      //enable the pull up register
        P1->DIR &= ~BIT1;     //p1.1 to be input
        P1->REN |= BIT1;      //Enable pull-up resistor for P1.1
        P1 ->IFG &= ~BIT1;
        P1->IE |= BIT1;       //Enable interrupt for P1.1
        P1->IES |= BIT1;      //Interrupt on high to low transition

              P5->SEL0 &= ~BIT1;    //
              P5->SEL1 &= ~BIT1;    //General I/O is enable for P1.1
              P5->OUT |= BIT1;      //enable the pull up register
              P5->DIR &= ~BIT1;     //p1.1 to be input
              P5->REN |= BIT1;      //Enable pull-up resistor for P1.1
              P5 ->IFG &= ~BIT1;
              P5->IE |= BIT1;       //Enable interrupt for P1.1
              P5->IES |= BIT1;      //Interrupt on high to low transition


        NVIC_EnableIRQ(PORT5_IRQn);
        NVIC_EnableIRQ(PORT1_IRQn);

}




void buffer_Full_Config(){
    /*rgb led on booster pack*/
    /*      J4.37   =   blue   =    P2.6
     *      J4.38   =   Green  =    P2.4
     *      J4.39   =   RED    =    P5.6
     *
     *       */
    P1SEL0 &= ~BIT0;
    P1SEL1 &= ~BIT0;

    P1DIR |=BIT0;




}

//buzzer is 2.7!!

void buzzer_Config(){
    P2SEL0 |= BIT7;
    P2SEL0 |= BIT7;
    P2DIR  |= BIT7; //output
    P2OUT  &= ~BIT7;
    uint8_t i = 0;
    for(i=0; i<=50; i++){
        P2OUT ^= BIT7;
    }

}



void PORT1_IRQHandler(){
    int delay;
    for(delay = 0; delay < 200; delay ++ ){}
    if (P1->IFG & BIT1  ){
        P1 -> IFG &=~ BIT1;
        UART_putchar_n("print!", 6);
        print(myBufferPTR);
        clear_Buffer(&myBufferPTR);

        //:TODO print buffer to uart.
    }
 }

void PORT5_IRQHandler(){
    int delay;
    for(delay = 0; delay < 200; delay ++ ){}
    if (P5->IFG & BIT1  ){
        P5 -> IFG &=~ BIT1;
        UART_putchar_n("print!", 6);
        print(myBufferPTR);
        clear_Buffer(&myBufferPTR);

        //:TODO print buffer to uart.
    }
 }
