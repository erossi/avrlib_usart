#!/usr/bin/make

MCU = atmega16
INC = -I/usr/avr/include/
LIBS = m
OPTLEV = s
export CFLAGS = $(INC) -Wall -Wstrict-prototypes -pedantic -mmcu=$(MCU) -O$(OPTLEV)
export LFLAGS = -l$(LIBS)
export CC = avr-gcc

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size
DUDE = avrdude -c stk500v1 -p $(MCU) -P /dev/ttyUSB0 -e -U flash:w:test.hex
REMOVE = rm -f
objects = uart_isr.o uart.o

.PHONY: clean
.SILENT: help
.SUFFIXES: .c, .o

all:
	echo "Undefined"

test_rx_int: uart_isr.o
	$(CC) $(CFLAGS) -D TXRXI -c uart.c
	$(CC) $(CFLAGS) -o test.elf test_rx_int.c $(objects) $(LFLAGS)
	$(OBJCOPY) test.elf test.hex

test_rxtx_int:
	$(CC) $(CFLAGS) -o test.elf test_rxtx_int.c $(objects) $(LFLAGS)
	$(OBJCOPY) test.elf test.hex

program:
	$(DUDE)

clean:
	$(REMOVE) *.elf *.hex *.o

