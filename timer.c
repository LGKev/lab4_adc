/*
 * timer.c
 *
 *  Created on: Sep 10, 2017
 *      Author: kk
 */

#include "timer.h"
#include "msp.h"

void timer_a0_config()
{
    __disable_irq();
#ifdef PROBLEM_9
    TIMER_A0->R = 0;         // Reset count, set to 0 at begining
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2|TIMER_A_CTL_ID__8| TIMER_A_CTL_MC__UP;//UP MODE, SOURCE SEL SMCLK, INTERRUPT ENABLE

    TIMER_A0->CCR[0] = 4727;// Value to count to, from lab questions 4727;
    TIMER_A0->CCTL[0] &= ~CCIFG;//clear compare captture flag
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;// INTERRUPT INABLE CAPTURE COMPARE
    TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_7;//divide by 8

#endif

//#ifdef RGB_TIMER_CYCLE_CONFIG
//    TIMER_A0->R = 0;         // Reset count, set to 0 at begining
//    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__8
//            | TIMER_A_CTL_MC__STOP;  //stop MODE, SOURCE SEL SMCLK, INTERRUPT ENABLE
//
//    TIMER_A0->CCR[0] = 11812;    // Value to count to, from lab questions
//    TIMER_A0->CCTL[0] &= ~CCIFG; //clear compare captture flag
//    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // INTERRUPT INABLE CAPTURE COMPARE
//    TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_7; //divide by 8
//#endif

    /* Enable Interrupts in the NVIC */
    NVIC_EnableIRQ(TA0_0_IRQn);
}

void timer_a0_configTimer1()
{
    __disable_irq();

    TIMER_A0->R = 0;         // Reset count, set to 0 at begining
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID__1
            | TIMER_A_CTL_MC__UP;  //UP MODE, SOURCE SEL SMCLK, INTERRUPT ENABLE


#ifdef TIMER_65000
    TIMER_A0->CCR[1] = 65000;    // Value to count to, from lab questions
#endif

#ifdef TIMER_40000
    TIMER_A0->CCR[1] = 40000;    // Value to count to, from lab questions
#endif

#ifdef TIMER_20000
    TIMER_A0->CCR[1] = 20000;    // Value to count to, from lab questions
#endif
#ifdef TIMER_1000
    TIMER_A0->CCR[1] = 1000;    // Value to count to, from lab questions
#endif

    TIMER_A0->CCTL[1] &= ~CCIFG; //clear compare captture flag
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE; // INTERRUPT INABLE CAPTURE COMPARE
    NVIC_EnableIRQ(TA0_N_IRQn);
}

void TA0_0_IRQHandler() //basically used only to cycle led colors
{

#ifdef RGB_TIMER_CYCLE_CONFIG
    TIMER_A0->CCTL[0] &= ~CCIFG; //clear compare captture flag

    P2OUT++;

  if(P2OUT > 8){
      P2OUT  = 0; //reset to off.
      TIMER_A0->CCR[0] = 0; //stop that timer.
  }


#endif

#ifdef TIMER_A_TEST
         P1->OUT ^= BIT7;
         TIMER_A0->CCTL[0] &= ~CCIFG;
#endif

#ifdef PROBLEM_9
         P1->OUT ^= BIT7;
         P1->OUT ^= BIT0;

         TIMER_A0->CCTL[0] &= ~CCIFG;
#endif

}

//so this is the hanlder we need, but unforunately my flag isn't set still!
void TA0_N_IRQHandler()
{ //dedicated for 1-5? and so i guess Ta_0_0 is the handler specifically for cctl[0]
    P1->OUT ^= BIT7;

    if (TIMER_A0->CCTL[1] & CCIFG)
    { //ABSOLUTELY necessary will not work otherwise.

        P1->OUT ^= BIT7;
        P1->OUT ^= BIT0;

    }

    TIMER_A0->CTL &= ~TIMER_A_CTL_IFG; //moved here b/c i think it was clearing flags too early. only clear 1 flag a time.

    TIMER_A0->CCTL[1] &= ~CCIFG;

}
