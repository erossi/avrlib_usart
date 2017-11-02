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

/*! \file usart_rxcbuffer.h
 * \brief RS232 - IO functions.
 */

#ifndef _USART_RXCBUFFER_H_
#define _USART_RXCBUFFER_H_

#include <stdint.h>
#include "usart_base.h"
#include "circular_buffer.h"

class Usart0_RxCBuffer : public Usart0_Base {
	private:
		static CBuffer<uint8_t, uint8_t> rxbuffer_ {RX_BUF_SIZE};
	public:
		static void resume();
		static void suspend();
		static bool getc(uint8_t *);
		static bool get(uint8_t *, const uint8_t);
		static void put(const uint8_t c) { Usart0_Base::put(c); };
		static void clear() { rxbuffer_.clear(); };
};

#endif
