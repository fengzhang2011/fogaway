CPP=g++
CC=gcc
RM=del

BIN=LabMan
OBJ=LabMan.o
SRC=LabMan.cpp LMP.cpp

BIN_PATH=.
INCLUDE_FLAG=-I../include

LIBS=

CXXFLAGS=-Wall
all: $(TARGET)

.PHONY: all all-before all-after clean clean-custom
all: all-before $(BIN) all-after

clean: clean-custom
	$(RM) *.o
	$(RM) $(BIN_PATH)\*.exe

$(BIN) : $(OBJ)
	$(CPP) -o $(BIN_PATH)/$@ $(OBJ) $(LIBS) -Wl,--kill-at
$(OBJ) : $(SRC)
	$(CPP) -c $(CXXFLAGS) $(INCLUDE_FLAG) $<

