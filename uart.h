/*
 * uart.h
 *
 *  Created on: Sep 24, 2017
 *      Author: kevinKuwata1
 */

#include <stdint.h>

#ifndef UART_H_
#define UART_H_

/*
 * Configures UART A0.
 *  Clock:
 *  Baud:
 *  Settings: No parity, no address, LSb First, 8 Bit, 1 Start, 1 Stop
 *  Blocking
 * */
void UART_config();

/*
 * Place  data into Tx Buffer
 * */
void UART_putchar(uint8_t tx_data);

/*
 *  Iterates through the array, puts into the Tx Buffer
 * */
void UART_putchar_n(uint8_t * data, uint32_t length);




#endif /* UART_H_ */
