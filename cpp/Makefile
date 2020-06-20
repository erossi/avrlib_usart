# Copyright (C) 2017 Enrico Rossi
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

# space opt
OPTLEV = s

MCU = atmega328p
FCPU = 16000000UL
# ex. 1284p 1Mhz
# MCU = atmega1284p
# FCPU = 1000000UL

PWD = $(shell pwd)
INC = -I/usr/lib/avr/include/

# Compilers
AR = avr-ar
CXX = avr-g++

# Flags
CXXFLAGS := $(INC) -Wall -pedantic -std=c++14 -mmcu=$(MCU) -O$(OPTLEV) -D F_CPU=$(FCPU)
LFLAGS = -lm

## git version
#
GIT_TAG = "$(shell git describe --tags --long --always)"
PRGNAME = $(PRG_NAME)_$(GIT_TAG)_$(MCU)
ELFNAME = $(PRGNAME)_$@

## Program DUDE
#
DUDEPORT = /dev/ttyACM0
DUDEDEV = arduino
DUDE = avrdude -p $(MCU) -e -U flash:w:$(PRGNAME).hex

OBJCOPY = avr-objcopy -j .text -j .data -O ihex
OBJDUMP = avr-objdump
SIZE = avr-size --format=avr --mcu=$(MCU) $(PRGNAME).elf

REMOVE = rm -f

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .cpp, .o

## Export variables used in sub-make
#
.EXPORT_ALL_VARIABLES: doc

### Libraries
common_o := \
	usart_base.o \
	usart_rxcbuffer.o

## Default
#
all: \
	test_echo \
	test_echo_rxcbuffer

test_echo: $(common_o)
	$(CXX) $(CXXFLAGS) -o $@.elf $@.cpp $(common_o) $(LFLAGS)
	$(OBJCOPY) $@.elf $@.hex

test_echo_rxcbuffer: $(common_o)
	$(CXX) $(CXXFLAGS) -o $@.elf $@.cpp $(common_o) $(LFLAGS)
	$(OBJCOPY) $@.elf $@.hex

program:
	$(DUDE) -c $(DUDEDEV) -P $(DUDEPORT)

## CleanUp and tools
#
clean:
	$(REMOVE) *.elf *.hex *.o $(objects)

version:
	# Last Git tag: $(GIT_TAG)

size:
	$(SIZE)
