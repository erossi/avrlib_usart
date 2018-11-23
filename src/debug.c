/* Copyright (C) 2014-2016 Enrico Rossi

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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "debug.h"

/*! Get a string.
 *
 * Overwrites the EOL with \0.
 *
 * \param s pre-allocated string space.
 * \param echo enable the echo on non-irq rx.
 * \return TRUE message is present, FALSE no message.
 */
uint8_t debug_getstr(char *s, const uint8_t ssize, const uint8_t echo)
{
	uint8_t len;

#if DEBUG_RX_USEIRQ == 1
	len = usart_getmsg(DEBUG_SERIAL_PORT, (uint8_t *)s, ssize);

	if (len) {
		*(s + len) = 0;

		if (echo)
			usart_printstr(DEBUG_SERIAL_PORT, s);
	}
#else
	uint8_t loop;

	len = 0;
	loop = TRUE;

	while (loop && len < ssize) {
		*(s + len) = usart_getchar(DEBUG_SERIAL_PORT, TRUE);

		if (echo)
			usart_putchar(DEBUG_SERIAL_PORT, *(s + len));

		if (*(s + len) == '\r')
			loop = FALSE;
		else
			len++;
	}

	/* end of string */
	*(s + len) = 0;
#endif

	return(len);
}

/*! Print a flash-stored string to the terminal.
 *
 * \param string MUST be a PSTR() string.
 */
void debug_print_P(PGM_P string)
{
	if (debug->active) {
		strncpy_P(debug->buffer, string, debug->size);
		/* safe termination of the string */
		debug->buffer[debug->size - 1] = 0;
		usart_printstr(DEBUG_SERIAL_PORT, NULL);
	}
}

/*! Print the string.
 *
 * The string to be printed can be stored in the
 * buffer. Since the buffer points to the usart buffer
 * there is no need to pass anything to the usart port.
 *
 * \param string if NULL print the buffer.
 */
void debug_print(const char *string)
{
	if (debug->active) {
		if (string)
			strncpy(debug->buffer, string, debug->size);

		usart_printstr(DEBUG_SERIAL_PORT, NULL);
	}
}

/*! Print the greetings and release number. */
static void hello(void)
{
        debug_print_P(PSTR("\n\n\n"));
	debug_print_P(PSTR(GITREL));
	debug_print_P(PSTR("\n"));
        debug_print_P(PSTR("GNU LGPL v2.1\n\n"));
}

/*! Resume call.
 *
 * \ingroup sleep_group
 */
void debug_resume(void)
{
#ifndef DEBUG_USART_SLAVE
	usart_resume(DEBUG_SERIAL_PORT);
#endif

	debug->active = 1;
}

/*! Suspend call.
 *
 * \ingroup sleep_group
 */
void debug_suspend(void)
{
	debug->active = 0;

#ifndef DEBUG_USART_SLAVE
	usart_suspend(DEBUG_SERIAL_PORT);
#endif
}

/*! Initialize the debug_t structure and ask if
 * debug is active.
 *
 * \note this function can be called many times
 */
void debug_init(void)
{
	if (!debug) {
		debug = malloc(sizeof(struct debug_t));
		debug->usart = usart_init(DEBUG_SERIAL_PORT);
		debug->buffer = debug->usart->tx;
		debug->size = debug->usart->tx_size;
		debug_resume();
		hello();
	}
}

void debug_shut(void)
{
	if (debug) {
		debug_suspend();
		usart_shut(DEBUG_SERIAL_PORT);
		debug->usart = NULL;
		debug->buffer = NULL;
		free(debug);
	}
}
