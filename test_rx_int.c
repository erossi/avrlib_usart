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

#include <stdint.h>
#include <avr/interrupt.h>
#include "default.h"
#include "uart.h"
#include <util/delay.h>

/* Globals */
volatile uint8_t uart_char, flag;

int
main (void)
{
  int i;

  DDRB = 127;
  PORTB = 0;
  uart_char = 0;
  flag = 0;

  uart_init ();
  sei ();

  for (;;)
    {
      if (flag)
	{
	  PORTB = ~uart_char;
	  flag = 0;
	  uart_putchar (uart_char);
	}

      for (i = 0; i < 50; i++)
	_delay_ms (10);
    }
}
