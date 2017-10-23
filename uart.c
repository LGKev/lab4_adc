/*
 * uart.c
 *
 *  Created on: Sep 24, 2017
 *      Author: kevinKuwata1
 */

#include <Circular_Buffer_8_bit.h>
#include "uart.h"
#include "msp.h"

#define BLUE_LED            (BIT2)

    extern CircBuf_t * myBufferPTR;
    extern count[5];

void UART_config(){
/*  Steps for Setting up UART Table 22.3.1
 *      Set UCSWRST
 *      Initialize all registers with UCSWRT = 1
 *      Configure Ports
 *      Clear UCSWST with software, aka clear that 1.
 *      Enable interrupts for UCRxie and UCTxie
 *
 *
 * Resets as all 0's so only the clock sources is the
 *  that need to be changed will change.
 *
 *  "Frame parameters" means the parity, stop bits, etc. but cuz all reset as 0's
 *  no worries.
 * */

//Set up pins.
    //Pin 2 is the RX           pin 3 is the TX
    //RX SETUP
    P1SEL0 |= BIT2;
    P1SEL1 &= ~BIT2;

    //TX SETUP
    P1SEL0 |= BIT3;
    P1SEL1 &= ~BIT3;


    // UART must be in reset mode to configure
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; //reset by setting to 1.


    EUSCI_A0->IFG = 0b0;
    EUSCI_A0->IE |= 0b01;//BIT1 | EUSCI_A_IFG_RXIFG; // set up interrupt enable for both Rx and Tx.




    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK; //frame parameter , enable interrupt on the RX
    //baud rate clock is 4 Mhz
    //what register is UCBR? it needs to be set as 26 is that the word one?
    EUSCI_A0->BRW = 26; //baud rate, so baud rate set at 26 with other settings will result in a rate of 9600


    // UCOS16 = 1,          UCbRx = 26;              UCBRF = 0 ;              UCBRSx = 0xB6
    EUSCI_A0->MCTLW|=  (EUSCI_A_MCTLW_OS16); //from table 22.3.13

    EUSCI_A0->MCTLW|= ((0xB600)) ;//| (EUSCI_A_MCTLW_OS16); //from table 22.3.13


    // CLEAR UCSWRST
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    //set up interrupt
       EUSCI_A0->IE |= UCRXIE ;//BIT1 | EUSCI_A_IFG_RXIFG; // set up interrupt enable for both Rx and Tx.

       NVIC_EnableIRQ(EUSCIA0_IRQn);

}


/*
 * Place Load data into Tx Buffer
 * */
void UART_putchar(uint8_t tx_data){
   while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = tx_data; //hoping 30 should be '0', for sure ascii
   // EUSCI_A_ifg
}

/*
 *  Iterates through the array, puts into the Tx Buffer
 * */
void UART_putchar_n(uint8_t * data, uint32_t length){
    //data is an array! so you can use pointer math to iterate through
   uint8_t index =0;

   for(index = 0; index <length ; index++){
       UART_putchar(data[index]);
      // for(txDelay = 0; txDelay < 300; txDelay++);
   }
}


/*Interrupt for UART Rx and TX IRQ */
extern void EUSCIA0_IRQHandler(){
    uint16_t delay;

    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        //when a bit is received the RXIF flag is high.
        P2OUT ^= BLUE_LED;
               for(delay =0; delay<200; delay++);
               P2OUT ^= BLUE_LED;
               EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;//clear the flag.

               //push into the buffer.
               add_To_Buffer(&myBufferPTR, EUSCI_A0->RXBUF);

               //echo that shit back
               //EUSCI_A0->TXBUF = remove_From_Buffer(&myBufferPTR);
#ifdef ECHO_TEST
               EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;
#endif
    }


    if(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG){
    }
}

