CC=gcc
CFLAGS=-g -O0 --std=c99

RM=rm
RMFLAGS=-f

OS=linux

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

OBJ=os/$(OS)/$(OS).o core/region.o core/stream.o

build : $(OBJ)

testing/test_os_input.bin : $(OBJ) testing/test_os_input.o
	$(CC) $(CFLAGS) $^ -o $@

testing/test_os_cursor.bin : $(OBJ) testing/test_os_cursor.o
	$(CC) $(CFLAGS) $^ -o $@

testing/test_os_attr.bin : $(OBJ) testing/test_os_attr.o
	$(CC) $(CFLAGS) $^ -o $@

testing/test_region.bin : $(OBJ) testing/test_region.o
	$(CC) $(CFLAGS) $^ -o $@

testing/test_stream.bin : $(OBJ) testing/test_stream.o
	$(CC) $(CFLAGS) $^ -o $@

test : testing/test_os_input.bin testing/test_os_cursor.bin testing/test_os_attr.bin testing/test_region.bin testing/test_stream.bin

clean :
	$(RM) $(RMFLAGS) $(OBJ) $(shell echo testing/*.bin testing/*.o)
