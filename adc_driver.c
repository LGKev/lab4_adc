/*
 * adc_driver.c
 *
 *  Created on: Oct 11, 2017
 *      Author: kk
 */

#include "msp.h"
#include "lab4.h"

void ADC_CONFIG(){
    //sample and conversion timing
    // sample input signal = SHI
    //ADC14SC bit


    /* Pins for the Accelerometer  */
    P6SEL0 = BIT1 ;  //A14
    P6SEL1 = BIT1;  //A14

    P4SEL0 = BIT0 | BIT2;  //A13 and A11
    P4SEL1 = BIT0 | BIT2;  //A13 and A11

#ifdef INTERNAL_TEMPERATURE_READ
    //Pins for temperature sensor Analog 22, port 8 pin 3.
    //P8SEL0 = BIT3;
    //P8SEL1 = BIT3;
    //Do I need to make port direction input? what about pull up resistor?

    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);
    REF_A->CTL0 |= REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;
    REF_A->CTL0 &= ~REF_A_CTL0_TCOFF;

    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP; //96 clocks, turn on, and sample and hold mode
    ADC14->CTL1 |= ADC14_CTL1_TCMAP | ADC14_CTL1_RES_3; //temp sensor, set resolution: 16 clock cycles

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_22 ; //MAP the temp to the first block of memory temperature sensor analog pin, table 6-52 from overview 100 pin PZ devices

    // need to enable interrupt for conversion of data.
    ADC14->IER0 |= ADC14_IER0_IE22; // allow interrupt for analog pin 22, or is this directly tied to the interrupt for mem0
    ADC14->IER0 |= ADC14_IER0_IE0; //interrupt for mem[0]


    //map the accelerometer x, y , and z for the mem1 mem2 and mem3

    NVIC_EnableIRQ(ADC14_IRQn); //a flag ADC14IFGx is set when x has a conversion result.
#endif



}


void ADC14_IRQHandler(){
    if(ADC14->IFGR0 &  ADC14_IFGR0_IFG22){
        //temperature sensor data ready to be processed
        // don't clear flag: "bit set to 0 when register read" (20.3.11)
        uint8_t var1 = 0;
    }
    if(ADC14->IFGR0 & ADC14_IFGR0_IFG0){

        uint32_t TS30 = TLV->ADC14_REF1P2V_TS30C;
        uint32_t TS85 = TLV->ADC14_REF1P2V_TS85C;
        uint32_t adcVal = ADC14->MEM[0];

        //set a break point to determine if the interrupt was for mem[0] or channel [22]
        float calculated_temp = (ADC14->MEM[0] - TLV->ADC14_REF1P2V_TS30C)*((85-30)/(TLV->ADC14_REF1P2V_TS85C- TLV->ADC14_REF1P2V_TS30C)) + 30;
        //reading 3623 in the converted. not sure if trust worthy. like will temp change? i placed finger and saw 0 change., placing on top of
        //computer to see if gets warmer.
        ///what does ADC14 at 14 bit resolution mean? why am i using mod clock?
        //well i think maybe it worked? i got a 3727 now after sitting on top of computer for a few minutes.

       //how to convert? reference is 1.2 v? max is 3.3v? or max 1.2 v? and ground is min?
        //my temp is now at 3719 converted.
        //it looks like maybe the referecnec voltage represents the top value. not the bottom. from 0 to 1.2v is the reference range.
    }
}


