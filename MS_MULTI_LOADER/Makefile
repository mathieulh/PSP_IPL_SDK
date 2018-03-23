TARGET = ipl_ms
OBJS = \
  ../IPL_SDK/crt0_ms.o \
\
  iplboot.o \
  fatload.o \
\
  ../IPL_SDK/cache.o \
  ../IPL_SDK/psp_uart.o \
  ../IPL_SDK/sysreg.o \
  ../IPL_SDK/syscon.o \
  ../IPL_SDK/memstkro.o \
  ../IPL_SDK/kprintf.o \
  ../IPL_SDK/kirk.o \
  ../IPL_SDK/interrupt.o \
\
  ../LIBC/memcmp.o \
  ../LIBC/memcpy.o \
  ../LIBC/memset.o \
  ../tff/diskio.o \
  ../tff/tff.o \

INCDIR = ../IPL_SDK ../tff_min ../tff
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = 
LIBDIR =
LDFLAGS = -nostartfiles -T ipl_mload.x -nostdlib

PSPSDK=$(shell psp-config --pspsdk-path)
#include $(PSPSDK)/lib/build.mak
include ../IPL_SDK/build_bin.mak
