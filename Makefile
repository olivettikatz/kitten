CC=gcc
CFLAGS=-g -O2 --std=c99

CXX=g++
CXXFLAGS=-g -O2 -std=c++11

AR=ar
ARFLAGS=rcs

RM=rm
RMFLAGS=-f --

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

PWD=$(shell pwd)
SRC=$(PWD)
KERNEL=$(PWD)/kernel
KERNEL_ARCH=$(KERNEL)/arch

ARCH_OBJ=$(KERNEL_ARCH)/x86.o
KERNEL_OBJ=$(KERNEL)/vector.o $(KERNEL)/generator.o
OBJ=$(ARCH_OBJ) $(KERNEL_OBJ) $(KERNEL_ARCH_OBJ) $(SRC)/builder.o

build : $(OBJ) $(KERNEL)/libnightowl_kernel.so $(SRC)/libnightowl.a

$(KERNEL)/libnightowl_kernel.so : $(KERNEL_OBJ)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libnightowl_kernel.so $^ -o $@

$(SRC)/libnightowl.a : $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

clean : 
	$(RM) $(RMFLAGS) $(OBJ) $(KERNEL)/libnightowl_kernel.so
