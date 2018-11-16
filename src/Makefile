# Copyright (C) 2014-2016 Enrico Rossi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

PRG_NAME = usart
OPTLEV = s

## Microcontroller definition
#
MCU = atmega328p
FCPU = 16000000UL

PWD = $(shell pwd)
INC = -I/usr/lib/avr/include/

CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -lm

# Uncomment if git is in use
GIT_TAG = "$(shell git describe --tags --long --always)"
PRGNAME = $(PRG_NAME)_$(GIT_TAG)_$(MCU)
ELFNAME = $(PRGNAME)_$@

AR = avr-ar
CC = avr-gcc

## Programming device
# Arduino
DUDEPORT = /dev/ttyACM0
DUDEDEV = arduino
#
## Stk500v2
#DUDEPORT = /dev/ttyUSB0
#DUDEDEV = stk500v2
#
## avrispmkII
#DUDEPORT = usb
#DUDEDEV = avrispmkII

# Use sudo for USB avrispmkII
DUDE = sudo avrdude -p $(MCU) -e -U flash:w:$(PRGNAME).hex

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size --format=avr --mcu=$(MCU) $(PRGNAME).elf

REMOVE = rm -f

usart_obj = circular_buffer.o usart.o
debug_obj = $(usart_obj) debug.o

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .c, .o

## Export variables used in sub-make
#
.EXPORT_ALL_VARIABLES: doc

## Default
#
all:
	echo "Undefined"

test: $(debug_obj)
	$(CC) $(CFLAGS) -o $(ELFNAME).elf test_rx_int.c \
		 $(debug_obj) $(LFLAGS)
	$(OBJCOPY) $(ELFNAME).elf $(PRGNAME).hex

## Circular Buffer
# -D CBUF_SIZE
# -D CBUF_EOM
#
circular_buffer.o:
	$(CC) $(CFLAGS) -D CBUF_SIZE=16 -D CBUF_EOM=0 -c circular_buffer.c

## Usart
# -D USE_USART1 to use the second serial port.
# -D USARTx_EOL for the End Of Line.
#
# $(CC) $(CFLAGS) -D USE_USART1 -D USART1_EOL=0x03 -c usart.c
#
usart.o:
	$(CC) $(CFLAGS) -D USE_ARDUINO -D USART0_EOL=0x30 -c usart.c

## Debug
# -D GITREL must be present, definition in the code.
# -D USE_USART1 if the debug works on the the serial port 1.
#
# $(CC) $(CFLAGS) -D USE_USART1 -D GITREL=\"$(GIT_TAG)\" -c debug.c
#
debug.o:
	$(CC) $(CFLAGS) -D GITREL=\"$(GIT_TAG)\" -c debug.c

## Programming part
#
program:
	$(DUDE) -c $(DUDEDEV) -P $(DUDEPORT)

## CleanUp and tools
#
clean:
	$(REMOVE) *.elf *.hex *.o $(objects)

version:
	# Last Git tag: $(GIT_TAG)

doc_timing:
	$(MAKE) -C ../doc timing

doc:
	$(MAKE) -C ../doc doc

size:
	$(SIZE)
