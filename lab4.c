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

