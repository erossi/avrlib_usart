/* Copyright (C) 2005-2016 Enrico Rossi

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

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usart.h"

/*! \brief Interrupt rx.
 *
 * This function is triggered every incoming char from the serial
 * port.
 *
 * If USART_CHARFILTER_0 is defined then only ASCII from 32 to 127
 * and 0 are stored.
 */
ISR(USART0_RX_vect)
{
	uint8_t rxc;

	/*! First copy the rx char from the device rx buffer. */
	rxc = UDR0;

#ifdef USART0_EOL
	if (rxc == USART0_EOL)
		rxc = 0;
#endif

#ifdef USART_CHARFILTER_0
	if (!rxc || ((rxc > 31) && (rxc < 128)))
		cbuffer_push(usart0->rx, rxc);
#else
	cbuffer_push(usart0->rx, rxc);
#endif
}

#ifdef USE_USART1
/*! Repeat it for the second serial port if used. */
ISR(USART1_RX_vect)
{
	uint8_t rxc;

	/*! First copy the rx char from the device rx buffer. */
	rxc = UDR1;

#ifdef USART1_EOL
	if (rxc == USART1_EOL)
		rxc = 0;
#endif

#ifdef USART_CHARFILTER_1
	if (!rxc || ((rxc > 31) && (rxc < 128)))
		cbuffer_push(usart1->rx, rxc);
#else
	cbuffer_push(usart1->rx, rxc);
#endif
}
#endif

/*! Start the usart port.
 *
 * See datasheet for more info, generally it is better to put fixed
 * parameter from the datasheet instead of calculating it.
 *
 * if it is possible to improve baud rate error by using 2x clk

#if defined(U2X0)
	UCSR0A = _BV(U2X0);
	UBRR0H = (uint8_t)((F_CPU / 8UL / USART0_BAUD - 1) >> 8);
	UBRR0L = (uint8_t)(F_CPU / 8UL / USART0_BAUD - 1);
#else
	UBRR0H = (uint8_t)((F_CPU / 16UL / USART0_BAUD - 1) >> 8);
	UBRR0L = (uint8_t)(F_CPU / 16UL / USART0_BAUD - 1);
#endif

 * example of initializations
 * Tx only without Rx
 * UCSR0B = _BV(TXEN0);
 * Tx and Rx only without interrupt
 * UCSR0B = _BV(TXEN0) | _BV(RXEN0);
 * Rx only with interrupt
 * UCSR0B = _BV(RXCIE0) | _BV(RXEN0);
 * Rx with interrupt and Tx normal
 * UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
 * Rx and Tx with interrupt
 * UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXCIE0) | _BV(TXEN0);
 *
 * \parameters port the serial port number.
 */
void usart_resume(const uint8_t port)
{
	if (port) {

#ifdef USE_USART1
		cbuffer_clear(usart1->rx);
		usart1->flags.all = 0;
		usart1->tx[0] = 0;

		/*! 9600 bps */
		UCSR1A = _BV(U2X1);
		UBRR1H = 0;
		UBRR1L = 12;

		/*! tx/rxI enable, 8n1 */
		UCSR1C = _BV(UCSZ10) | _BV(UCSZ11);
		UCSR1B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1);
#endif

	} else {
		cbuffer_clear(usart0->rx);
		usart0->flags.all = 0;
		usart0->tx[0] = 0;

		/*! 115200 bps */
		UCSR0A = _BV(U2X0);
		UBRR0H = 0;
		UBRR0L = 16;
		/*! tx/rxI enable, 8n1 */
		UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
		UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	}
}

/*! Disable the usart port. */
void usart_suspend(const uint8_t port)
{
	if (port) {
#ifdef USE_USART1
		UCSR1B = 0;
		usart_getchar(1, FALSE);
		/* I do not care about resetting all the parameters.
		UCSR1C = 0;
		UBRR1L = 0;
		UCSR1A &= ~_BV(U2X1);
		*/
#endif
	} else {
		UCSR0B = 0;
		usart_getchar(0, FALSE);
		/* I do not care about resetting all the parameters.
		UCSR0C = 0;
		UBRR0L = 0;
		UCSR0A &= ~_BV(U2X0);
		*/
	}
}

/*! Init the usart port.
 *
 * Allocates the global usart pointer.
 *
 * \note does NOT start the port(s).
 * \warning this should be called only once in the code
 * to avoid memory leak.
 */
void usart_init(const uint8_t port)
{
	if (port) {
#ifdef USE_USART1
		usart1 = malloc(sizeof(struct usart_t));
		usart1->rx = cbuffer_init(TRUE);
		usart1->tx = malloc(USART1_TXBUF_SIZE);
#endif
	} else {
		usart0 = malloc(sizeof(struct usart_t));
		usart0->rx = cbuffer_init(TRUE);
		usart0->tx = malloc(USART0_TXBUF_SIZE);
	}
}

/*! Get a char directly from the usart port. */
char usart_getchar(const uint8_t port, const uint8_t locked)
{
#ifdef USE_USART1
	if (locked) {
		if (port) {
			loop_until_bit_is_set(UCSR1A, RXC1);
			return(UDR1);
		} else {
			loop_until_bit_is_set(UCSR0A, RXC0);
			return(UDR0);
		}
	} else {
		if (port) {
			if (bit_is_set(UCSR1A, RXC1))
				return(UDR1);
			else
				return(FALSE);
		} else {
			if (bit_is_set(UCSR0A, RXC0))
				return(UDR0);
			else
				return(FALSE);
		}
	}
#else
	if (locked) {
		loop_until_bit_is_set(UCSR0A, RXC0);
		return(UDR0);
	} else {
		if (bit_is_set(UCSR0A, RXC0))
			return(UDR0);
		else
			return(FALSE);
	}
#endif
}

void usart_clear_rx_buffer(const uint8_t port)
{
#ifdef USE_USART1
	if (port) {
		cbuffer_clear(usart1->rx);
	} else {
		cbuffer_clear(usart0->rx);
	}
#else
	cbuffer_clear(usart0->rx);
#endif
}

/*! get the message from the RX buffer (legacy).
 *
 * See usart_getnmsg() for info.
 *
 * \param port the serial port.
 * \param s the string to copy the message to.
 */
uint8_t usart_getmsg(const uint8_t port, char *s)
{
#ifdef USE_USART1
	if (port)
		return(cbuffer_popm(usart1->rx, s, USART1_RXBUF_SIZE));
	else
		return(cbuffer_popm(usart0->rx, s, USART0_RXBUF_SIZE));
#else
	return(cbuffer_popm(usart0->rx, s, USART0_RXBUF_SIZE));
#endif
}

/*! get the message from the RX buffer of a given maxsize.
 *
 * \param port the serial port.
 * \param s the string to copy the message to.
 * \param size the sizeof(s).
 * \note s must have the allocated size, safety termination is in place.
 * \bug this function should be atomic, cannot be interrupted
 * while resetting the pointer.
 */
uint8_t usart_getnmsg(const uint8_t port, char *s, const uint8_t size)
{
	uint8_t ok;

#ifdef USE_USART1
	if (port)
		ok = cbuffer_popm(usart1->rx, s, size);
	else
		ok = cbuffer_popm(usart0->rx, s, size);
#else
	ok = cbuffer_popm(usart0->rx, s, size);
#endif

	*(s + size - 1) = 0;
	return(ok);
}

/*! Send character c down the USART Tx, wait until tx holding register
 * is empty.
 *
 * \parameter port the serial port.
 * \parameter c the char to send.
 */
void usart_putchar(const uint8_t port, const char c)
{
#ifdef USE_USART1
	if (port) {
		loop_until_bit_is_set(UCSR1A, UDRE1);
		UDR1 = c;
	} else {
		loop_until_bit_is_set(UCSR0A, UDRE0);
		UDR0 = c;
	}
#else
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
#endif
}

/*! Send a C (NUL-terminated) string down the USART Tx.
 *
 * \parameter port the serial port.
 * \parameter s the string to send. If there is no string then
 * send the content of the tx_buffer.
 * \note The tx_buffer left unclean.
 */
void usart_printstr(const uint8_t port, const char *s)
{
#ifdef USE_USART1
	/* If no char then print the buffer */
	if (!s) {
		if (port)
			s = usart1->tx;
		else
			s = usart0->tx;
	}
#else
	/* If no char then print the buffer */
	if (!s)
		s = usart0->tx;
#endif

	while (*s) {
#ifdef DOS_CRLF
		if (*s == '\n')
			usart_putchar(port, '\r');
#endif
		usart_putchar(port, *s++);
	}
}
