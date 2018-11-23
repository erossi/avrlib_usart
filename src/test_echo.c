/* Copyright (C) 2017 Enrico Rossi

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

/*! \file test_usart1_echo.c
 *
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include "usart.h"

int main(void)
{
	uint8_t c;

	sei();
	usart_init(0);
	usart_resume(0);
	usart_printstr(0, "USART0 Test echo \n");

	while (1) {
		if (usart_get(0, &c, 1))
			usart_putchar(0, c);
	}

	return(0);
}
