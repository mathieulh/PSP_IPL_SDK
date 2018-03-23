TARGET = pspboot
OBJS = \
  ../IPL_SDK/crt0_ipl.o \
  main.c \
\
  ../IPL_SDK/sysreg.o \
  ../IPL_SDK/syscon.o \
\

INCDIR = ../IPL_SDK ../tff
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = 
LIBDIR =
LDFLAGS = -nostartfiles -T payloadipl.x -nostdlib

PSPSDK=$(shell psp-config --pspsdk-path)
#include $(PSPSDK)/lib/build.mak
include ../IPL_SDK/build_bin.mak
