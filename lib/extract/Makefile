include ../config.mk

OBJ+=extract.o
TEST+=test_extract.bin

build : $(OBJ) main.o
	$(AR) $(ARFLAGS) libextract.a $(OBJ)
	$(CXX) $(CXXFLAGS) main.o -o kitten-extract-doc -lextract -largs

test: $(TEST)

clean :
	$(RM) $(RMFLAGS) $(OBJ) $(TEST) libextract.a main.o kitten-extract-doc
