/*
    USART - Serial port library.
    Copyright (C) 2005-2017 Enrico Rossi

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

	/*! First copy the rx char from the device rx buffer. */
	rxc = UDR0;
	Usart0_RxCBuffer::put(rxc);
}

/*! Start the usart port.
 */
void Usart0_RxCBuffer::resume()
{
	rxbuffer_.clear();
	Usart0_Base::resume();
}

/*! Disable the usart port. */
void Usart0_RxCBuffer::suspend()
{
	Usart0_Base::suspend();
	rxbuffer_.clear();
}

bool Usart0_RxCBuffer::getc(uint8_t *data)
{
	return(rxbuffer_.popc(data));
}

bool Usart0_RxCBuffer::get(uint8_t *data, const uint8_t sizeofdata)
{
	return(rxbuffer_.pop(data, sizeofdata));
}
