/*
    USART - Serial port library.
    Copyright (C) 2005-2019 Enrico Rossi

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
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usart_rxcbuffer.h"

/*! \brief Interrupt rx.
 *
 * IRQ functions triggered every incoming char from the serial
 * ports.
 * If USARTn_EOL defined, every incoming USARTn_EOL chars increments
 * the message counter in the usartn struct.
 *
 */
ISR(USART0_RX_vect)
{
	uint8_t rxc;

	rxc = UDR0; // Get the char from the device
	Usart0_RxCBuffer::rxbuffer.push(rxc); // push it into the buffer
}

/*! Out of class cbuffer constructor.
 *
 * Since we use a static class, allocate the space for
 * the buffer.
 */
CBuffer<uint8_t, uint8_t> Usart0_RxCBuffer::rxbuffer;

/*! Start the usart port.
 *
 * Override the base resume, we need to use the RX IRQ.
 */
void Usart0_RxCBuffer::resume()
{
	rxbuffer.clear();
	UCSR0A = _BV(U2X0);
	UBRR0H = 0;
	UBRR0L = 207;
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); // 8n1
	// Rx with interrupt and Tx normal
	UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
}

/*! Disable the usart port. */
void Usart0_RxCBuffer::suspend()
{
	Usart0_Base::suspend();
	rxbuffer.clear();
}

uint8_t Usart0_RxCBuffer::get(uint8_t *data, const uint8_t sizeofdata)
{
	return(rxbuffer.pop(data, sizeofdata));
}

void Usart0_RxCBuffer::put(const uint8_t c)
{
	Usart0_Base::put(c);
}

void Usart0_RxCBuffer::clear()
{
	rxbuffer.clear();
}
