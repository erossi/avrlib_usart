/*
    Copyright (C) 2016 Enrico Rossi

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "debug.h"

void print_usart(void)
{
	sprintf(debug->buffer, "i: %d | ", usart0->rx->idx);
	debug_print(NULL);
	sprintf(debug->buffer, "s: %d | ", usart0->rx->start);
	debug_print(NULL);
	sprintf(debug->buffer, "o: %d", usart0->rx->overflow);
	debug_print(NULL);
	debug_print_P(PSTR("\n"));
}

void print_buffer(void)
{
	uint8_t *buf;
	uint8_t i, j;

	buf = malloc(20);

	if (usart0->rx->len) {
		debug_print_P(PSTR("Message: "));

		if (usart_getmsg(0, buf, 20))
			debug_print((char *)buf);
	} else {
		/* binary data */
		j = usart_get(0, buf, 20);
		sprintf(debug->buffer, "Data %02d: ", j);
		debug_print(NULL);

		for (i=0; i<j; i++) {
			sprintf(debug->buffer, " %02x", *(buf+i));
			debug_print(NULL);
		}
	}

	debug_print_P(PSTR("\n"));
	free(buf);
}

int main(void) {
	uint8_t i = 0;

	debug_init();
	sei();

	while (1) {
		sprintf(debug->buffer, "%02d: ", i);
		debug_print(NULL);
		print_usart();

		if (i > 10) {
			print_buffer();
			i=0;
		}

		_delay_ms(1000);
		i++;
	}

	return(0);
}
