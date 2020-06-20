/*
    USART - Serial port library.
    Copyright (C) 2005-2018 Enrico Rossi

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301  USA
 */

#include <stdint.h>
#include <avr/io.h>
#include "usart_base_p2.h"

void Usart1_Base::resume()
{
	UCSR1A = _BV(U2X1);
	UBRR1H = 0;
	UBRR1L = 207;
	UCSR1C = _BV(UCSZ10) | _BV(UCSZ11); // 8n1
	UCSR1B = _BV(RXEN1) | _BV(TXEN1);   // tx/rx
}

/*! Disable the usart port. */
void Usart1_Base::suspend()
{
	UCSR1B = 0;
	/* I do not care about resetting all the parameters.
	   UCSR1C = 0;
	   UBRR1L = 0;
	   UCSR1A &= ~_BV(U2X1);
	   */
}

/*! Get a byte directly from the usart port. */
bool Usart1_Base::get(uint8_t* data, const bool locked)
{
	if (locked) {
		loop_until_bit_is_set(UCSR1A, RXC1);
		*data = UDR1;
		return(true);
	} else {
		if (bit_is_set(UCSR1A, RXC1)) {
			*data = UDR1;
			return(true);
		} else {
			return(false);
		}
	}
}

/*! Send character c down the USART Tx
 *
 * \see Usart0_Base::put
 */
void Usart1_Base::put(const uint8_t c)
{
	// Wait for empty transmit buffer
	loop_until_bit_is_set(UCSR1A, UDRE1);
	UDR1 = c;
}
