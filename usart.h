/* Copyright (C) 2014, 2015 Enrico Rossi

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

/*! \file usart.h
 * \brief RS232 - IO functions.
 */

#ifndef _USART_H_
#define _USART_H_

/*! Arduino setup */
#ifdef USE_ARDUINO

#ifdef USE_USART1
#error You cannot use USE_USART1 with USE_ARDUINO
#endif

#define USART0_RX_vect USART_RX_vect
#define USART0_TX_vect USART_TX_vect

#endif

/*! Use the circular buffer
 */
#include "circular_buffer.h"
#define USART0_RXBUF_SIZE CBUF_SIZE
#define USART1_RXBUF_SIZE CBUF_SIZE

/*! USART 0 */
#define USART0_BAUD 115200
#define USART0_TXBUF_SIZE 64

/*! if the second serial port is in use */
#ifdef USE_USART1
#define USART1_BAUD 9600
#define USART1_TXBUF_SIZE 64
#endif

#ifndef CR
#define CR 0xd
#define LF 0xa
#endif

#ifndef USART0_EOL
#define USART0_EOL CR
#define USART0_EOL_YES
#endif

#ifndef USART1_EOL
#define USART1_EOL CR
#define USART1_EOL_YES
#endif

/*! Dos add CR to an LF */
#define DOS_CRLF

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*! Structure with IO buffers and indexes */
struct usart_t {
	/*! receive struct. */
	struct cbuffer_t *rx;
	/*! transmit buffer. */
	char *tx;
	/*! flags. */
	volatile union {
		/* GNU GCC only */
		__extension__ struct {
			uint8_t b2:6;
			uint8_t b1:1;
			uint8_t b0:1;
		} value;

		uint8_t all;
	} flags;
};

/*! Filter incoming char per serial port? */
#define USART_CHARFILTER_0
#define USART_CHARFILTER_1

/*! Global USART rxtx buffers pointer used inside the ISR routine. */
volatile struct usart_t *usart0;

#ifdef USE_USART1
volatile struct usart_t *usart1;
#endif

void usart_resume(const uint8_t port);
void usart_suspend(const uint8_t port);
void usart_init(uint8_t port);
char usart_getchar(const uint8_t port, const uint8_t locked);
void usart_putchar(const uint8_t port, const char c);
void usart_printstr(const uint8_t port, const char *s);
uint8_t usart_getmsg(const uint8_t port, char *s);
uint8_t usart_getnmsg(const uint8_t port, char *s, const uint8_t size);
void usart_clear_rx_buffer(const uint8_t port);

#endif
