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

/*! Get a string terminated with \r.
 *
 * \param s pre-allocated string space.
 * \param echo enable the echo on non-irq rx.
 * \return TRUE message is present, FALSE no message.
 */
uint8_t debug_getstr(char *s, const uint8_t size, const uint8_t echo)
{
#if DEBUG_RX_USEIRQ == 1
	if (usart_getmsg(DEBUG_SERIAL_PORT, s, size)) {

		if (echo)
			usart_printstr(DEBUG_SERIAL_PORT, s);

		return(TRUE);
	} else {
		return(FALSE);
	}
#else
	uint8_t i, loop;
       
	i = 0;
	ok = FALSE;

	while (!ok && i < size) {
		*(s + i) = usart_getchar(DEBUG_SERIAL_PORT, TRUE);

		if (echo)
			usart_putchar(DEBUG_SERIAL_PORT, *(s + i));

		if (*(s + i) == '\r')
			ok = TRUE;
		else
			i++;
	}

	/* safe end of string */
	*(s + i) = 0;
	return(ok);
#endif
}

/*! Print a flash-stored string to the terminal.
 *
 * \param string MUST be a PSTR() string.
 */
void debug_print_P(PGM_P string)
{
	if (debug->active) {
		strcpy_P(debug->buffer, string);
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
			strcpy(debug->buffer, string);

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
	usart_resume(DEBUG_SERIAL_PORT);
	debug->active = 1;
}

/*! Suspend call.
 *
 * \ingroup sleep_group
 */
void debug_suspend(void)
{
	debug->active = 0;
	usart_suspend(DEBUG_SERIAL_PORT);
}

/*! Initialize the debug_t structure and ask if
 * debug is active.
 */
void debug_init(void)
{
	debug = malloc(sizeof(struct debug_t));
	debug->usart = usart_init(DEBUG_SERIAL_PORT);
	debug->buffer = debug->usart->tx;
	debug_resume();
	hello();
}

void debug_shut(void)
{
	debug_suspend();
	usart_shut(DEBUG_SERIAL_PORT);
	debug->usart = NULL;
	debug->buffer = NULL;
	free(debug);
}
