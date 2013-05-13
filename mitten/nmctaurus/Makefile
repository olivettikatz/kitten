include config.mk

CFLAGS+=-I../../build -L../../build
CXXFLAGS+=-I../../build -L../../build
LIBS+=-lktp

OBJ+=parsing.o
TEST+=test_parsing.bin

build : $(OBJ)

test : $(TEST)

clean :
	$(RM) $(RMFLAGS) $(OBJ) $(TEST)
