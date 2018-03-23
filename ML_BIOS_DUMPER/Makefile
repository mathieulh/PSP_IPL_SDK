TARGET = pspboot
OBJS = \
  ../IPL_SDK/crt0_ipl.o \
\
  main.o \
  fatload.o \
\
  ../IPL_SDK/cache.o \
  ../IPL_SDK/psp_uart.o \
  ../IPL_SDK/sysreg.o \
  ../IPL_SDK/syscon.o \
  ../IPL_SDK/memstk.o \
  ../IPL_SDK/kirk.o \
  ../IPL_SDK/interrupt.o \
  ../IPL_SDK/kprintf.o \
\
  ../LIBC/memcmp.o \
  ../LIBC/memcpy.o \
  ../LIBC/memset.o \
\
  ../tff/diskio.o \
  ../tff/tff.o \

INCDIR = ../IPL_SDK ../tff
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = 
LIBDIR =
LDFLAGS = -nostartfiles -T ../IPL_SDK/pspipl.x -nostdlib

PSPSDK=$(shell psp-config --pspsdk-path)
#include $(PSPSDK)/lib/build.mak
include ../IPL_SDK/build_bin.mak
