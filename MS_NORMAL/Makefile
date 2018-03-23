TARGET = ipl_ms
OBJS = \
  ../IPL_SDK/crt0_ms.o \
  iplboot.o \
\
  ../IPL_SDK/sysreg.o \
  ../IPL_SDK/syscon.o \
\

#  ../IPL_SDK/psp_uart.o \
#  ../IPL_SDK/kprintf.o \
#\

INCDIR = ../IPL_SDK ../tff
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = 
LIBDIR =
LDFLAGS = -nostartfiles -T ipl_ms.x -nostdlib

PSPSDK=$(shell psp-config --pspsdk-path)
#include $(PSPSDK)/lib/build.mak
include ../IPL_SDK/build_bin.mak
