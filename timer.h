/*
 * timer.h
 *
 *  Created on: Sep 10, 2017
 *      Author: kk
 */

#ifndef TIMER_H_
#define TIMER_H_

/* Configures the timer for rgb cycling.
 * uses CCR[1] as the compare capture value;
 *
 * changes: CCR[0] value;
 *
 * */
void timer_a0_config(void);

/*
 * Configured this to determine what ccr[1] values due
 * for a clock and how it affects the p1.7 output
 *
 * */
void timer_a0_configTimer1();

void sysTickConfig(void);



#endif /* TIMER_H_ */
