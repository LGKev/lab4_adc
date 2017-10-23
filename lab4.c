/*
 * lab4.c
 *
 *  Created on: Oct 11, 2017
 *      Author: kk
 */

#include "math.h"
#include "lab4.h"
#include "msp.h"

float voltage_from_ADC(uint16_t nadc, float vref, uint8_t resolution_number_of_bits){

    float voltage = vref*((float)nadc/pow(2,resolution_number_of_bits));

    return voltage;
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
}


void PORT1_IRQHandler(){
    int delay;
    for(delay = 0; delay < 200; delay ++ ){}
    if (P1->IFG & BIT1  ){
        P1 -> IFG &=~ BIT1;
        clear_Buffer(&myBufferPTR);

        //:TODO print buffer to uart.
    }
     if(P1->IFG & BIT4){
        P1 ->IFG &=~BIT4;
        print(myBufferPTR);
        P2OUT ^= BIT2;
    }
 }
