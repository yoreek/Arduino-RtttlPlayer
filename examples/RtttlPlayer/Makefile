# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

#ARDUINO_QUIET = 1
ROOT_DIR = $(HOME)/robot

BOARD_TAG = uno
#BOARD_TAG    = mega
#BOARD_SUB = atmega2560

ARDUINO_DIR = $(ROOT_DIR)/arduino
AVR_TOOLS_DIR = /usr
AVRDUDE_CONF = /etc/avrdude.conf
ARDUINO_PORT = /dev/ttyACM0

# No reset and no init modem
#MONITOR_CMD = picocom -l -i -r
MONITOR_CMD = picocom -l

ARDUINO_LIBS = StateMachine RtttlPlayer

CXXFLAGS=-std=c++11 -std=c++1y -fno-move-loop-invariants -Wall -Wno-uninitialized -Werror -pedantic \
         -U__PROG_TYPES_COMPAT__
# Enable float type on sprintf
#LDFLAGS=-lprintf_flt -lm
# Fix bug with segfault during compilation
#CXXFLAGS=-fno-move-loop-invariants

include $(ROOT_DIR)/Arduino-Makefile/Arduino.mk
