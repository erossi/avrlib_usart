/*
 * Copyright (C) 2009 Enrico Rossi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _DEFAULT_H_
#define _DEFAULT_H_

/*
 * Delay.h CPU speed definition
 */

/* CPU Speed in Hz */
#define F_CPU 4000000UL
/*
 #define F_CPU 4000000UL 4Mhz
 #define F_CPU 14.7456E6

 maximum delay possible is: (see util/delay.h)
 The maximal possible delay is 262.14 ms / F_CPU in MHz.
 so we have 4Mhz CPU, maximum is 65ms. (65.535 actually).
*/

#define UART_BAUD 9600
#define UART_RXBUF_SIZE 128
#define UART_TXBUF_SIZE 128

struct uartStruct
{
  char *uart_rx_buffer;
  char *uart_tx_buffer;
  volatile uint8_t rx_flag, tx_flag, rx_msg, tx_msg;
};

#endif

