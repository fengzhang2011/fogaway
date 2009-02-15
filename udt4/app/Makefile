C++ = g++

ifndef arch
   arch = IA32
endif

CCFLAGS = -Wall -DWIN32 -DPTHREAD_FOR_WIN32 -DUDT_EXPORTS -I../src -O2 -finline-functions

ifeq ($(arch), IA32)
   CCFLAGS += -DIA32 #-mcpu=pentiumpro -march=pentiumpro -mmmx -msse
endif

ifeq ($(arch), POWERPC)
   CCFLAGS += -mcpu=powerpc
endif

ifeq ($(arch), IA64)
   CCFLAGS += -DIA64
endif

LDFLAGS = -L../src -ludt -lstdc++ -lpthreadGCE2 -lws2_32 -lm

OBJS = appserver.o appclient.o sendfile.o recvfile.o

DIR = $(shell pwd)

all: appserver appclient sendfile recvfile

%.o: %.cpp
	$(C++) $(CCFLAGS) $< -c

appserver: appserver.o
	$(C++) $^ -o $@ $(LDFLAGS)
appclient: appclient.o
	$(C++) $^ -o $@ $(LDFLAGS)
sendfile: sendfile.o
	$(C++) $^ -o $@ $(LDFLAGS)
recvfile: recvfile.o
	$(C++) $^ -o $@ $(LDFLAGS)

clean:
	del *.o
	del appserver.exe appclient.exe sendfile.exe recvfile.exe
