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

#include <inttypes.h>
#include <avr/interrupt.h>
#include "default.h"
#include "uart_isr.h"

/* Global variable and pointer to be used */
/* inside the ISR routine */

extern struct uartStruct *uartPtr;

ISR (USART_RXC_vect)
{
  char tmp;

  tmp = UDR;
  uartPtr->uart_rx_buffer[0] = tmp;
  uartPtr->rx_flag++;
}

ISR (USART_TXC_vect)
{
}

