/*
    Circular Buffer, a string oriented circular buffer implementation.
    Copyright (C) 2015, 2016 Enrico Rossi

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

#include <stdint.h>

#ifndef CBUFFER_H
#define CBUFFER_H

/*! Options DEFs
 *
 * -D CBUF_OVR_CHAR='X'
 * -D CBUF_EOM=0
 * -D CBUF_SIZE=16
 */

/*! End Of Message.
 */
#ifndef CBUF_EOM
#define CBUF_EOM 0
#endif

/*! The size of the buffer.
 */
#ifndef CBUF_SIZE
#define CBUF_SIZE 0x10
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

struct cbuffer_t {
	uint8_t *buffer;
	uint8_t idx;
	uint8_t start;
	uint8_t TOP;

	union {
		/* GNU GCC only */
		__extension__ struct {
			uint8_t msgs:6;
			uint8_t eom_plug:1;
			uint8_t overflow:1;
		} value;

		uint8_t all;
	} flags;

	uint8_t (*preprocess_rx)(struct cbuffer_t *cbuffer, char rxc);
};

void cbuffer_clear(struct cbuffer_t *cbuffer);
struct cbuffer_t *cbuffer_init(uint8_t plugin);
void cbuffer_shut(struct cbuffer_t *cbuffer);
uint8_t cbuffer_pop(struct cbuffer_t *cbuffer, uint8_t *data, const uint8_t size);
uint8_t cbuffer_popm(struct cbuffer_t *cbuffer, char *message, const uint8_t size);
uint8_t cbuffer_push(struct cbuffer_t *cbuffer, char rxc);

#endif
