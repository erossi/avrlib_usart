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

/*! \file debug.h
 * \brief Functions to activare and display (rs232) debug.
 */

#ifndef DBG_H
#define DBG_H

#include "usart.h"

#ifdef USE_DEFAULT_H
#include "default.h"
#endif

/*! Serial port. */
#ifndef DEBUG_SERIAL_PORT
#define DEBUG_SERIAL_PORT 0
#endif

/*! DEBUG_BUF_SIZE must be == USARTx_TXBUF_SIZE */
#ifndef DEBUG_BUF_SIZE
#define DEBUG_BUF_SIZE 16
#endif

/*! rx use the IRQ. */
#ifndef DEBUG_RX_USEIRQ
#define DEBUG_RX_USEIRQ 1
#endif

/*! GITREL Environment check */
#ifndef GITREL
#define GITREL "unknown"
#endif

/*! True False */
#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif

/*! debug structure, it has to be allocated,
  eventually, if debug is not active, you can avoid the
  allocation of the two members char *.
  */
struct debug_t {
	uint8_t active;
	char *buffer;
	volatile struct usart_t *usart;
};

/*! Global */
struct debug_t *debug;

uint8_t debug_getstr(char *s, const uint8_t size, const uint8_t echo);
void debug_print_P(PGM_P string);
void debug_print(const char *string);
void debug_resume(void);
void debug_suspend(void);
void debug_init(void);
void debug_shut(void);

#endif
