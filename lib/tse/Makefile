include ../config.mk

OS=linux

OBJ+=os/$(OS)/$(OS).o core/region.o core/stream.o core/async.o
TEST+=testing/test_os_input.bin testing/test_os_cursor.bin testing/test_os_attr.bin testing/test_region.bin testing/test_stream.bin

build : $(OBJ)
	$(AR) $(ARFLAGS) libtse.a $(OBJ)

test : $(TEST)

clean :
	$(RM) $(RMFLAGS) $(OBJ) $(TEST) libtse.a
