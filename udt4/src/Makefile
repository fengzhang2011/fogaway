C++ = g++

ifndef arch
   arch = IA32
endif

CCFLAGS = -Wall -DWIN32 -DPTHREAD_FOR_WIN32 -DUDT_EXPORTS -finline-functions -O3 -I../include/ #-msse3

ifeq ($(arch), IA32)
   CCFLAGS += -DIA32
endif

ifeq ($(arch), POWERPC)
   CCFLAGS += -mcpu=powerpc
endif

ifeq ($(arch), SPARC)
   CCFLAGS += -mcpu=sparc
endif

ifeq ($(arch), IA64)
   CCFLAGS += -DIA64
endif

ifeq ($(arch), AMD64)
   CCFLAGS += -DAMD64
endif

OBJS = md5.o common.o udtwindow.o list.o buffer.o packet.o channel.o queue.o ccc.o co-op.o core.o api.o
DIR = $(shell pwd)

all: udt.dll libudt.a udt

%.o: %.cpp %.h udt.h
	$(C++) $(CCFLAGS) $< -c

udt.dll: $(OBJS)
	$(C++) -shared -o $@ $^ -lstdc++ -lpthreadGCE2 -lws2_32 -lm

libudt.a: $(OBJS)
	ar -rcs $@ $^
	copy libudt.a "C:/mingw/lib/"

udt:
	copy udt.h udt

clean:
	del *.o *.dll *.a udt
