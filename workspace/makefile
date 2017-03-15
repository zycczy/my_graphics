#
#Author:charles cheng
#Date:2016-07-21
#
GCC := gcc

CFLAGS  := -g -Wall -O3 #each parameter mean: -g(for gdb), -Wall(show all warning), -O3(compile optimize on 3rd grade)

SDK_ROOT := ./..

INC := -I $(SDK_ROOT)/include \
       -I $(SDK_ROOT)/include/elibs \
       -I ./
SRC := $(wildcard ./*.c)#wildcard: use * for variable
OBJ := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))

DYNAMIC_LIB := graphics_lib.a
LIB         := $(SDK_ROOT)/elibs/$(DYNAMIC_LIB) \
               -lm
TARGET      := app
all:
	$(GCC) $(CFLAGS) $(INC) -c $(SRC) 
	$(GCC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIB)  
	rm *.o
clean:
	rm -f $(TARGET)

