#
#Author:charles cheng
#Date:2016-07-21
#
GCC := gcc
AR  := ar

ARFLAGS := -rcs        #each parameter mean: -r(Replace or add the specified files to the .a), -c(create .a), -s(create the symbol of .a)

SDK_ROOT := ./..

INC := -I $(SDK_ROOT)/include \
       -I ./
CFLAGS  := -g -Wall -O3 $(INC)#each parameter mean: -g(for gdb), -Wall(show all warning), -O3(compile optimize on 3rd grade)
SRC := $(wildcard ./*.c)#wildcard: use * for variable
OBJ := $(patsubst %.c, ./%.o, $(notdir ${SRC})) #patsubst:replace the *; notdir:except the path of dir

LIB_NAME := graphics_lib

DYNAMIC_LIB := $(LIB_NAME).a
LIB_PATH    := $(SDK_ROOT)/elibs

all:$(OBJ)
#	$(GCC) $(CFLAGS) $(INC) -c -o $(OBJ) $(SRC)
	$(AR) $(ARFLAGS) $(DYNAMIC_LIB) $(OBJ) #$^ means $(OBJ)
#	./%.o:./%.c
#	$(GCC) $(CFLAGS) -o $@ -c $< ${INC} -g
	mv $(DYNAMIC_LIB) $(LIB_PATH)
	rm *.o

clean:
	rm -f $(LIB_PATH)/$(DYNAMIC_LIB)

