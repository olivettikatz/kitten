CXX=g++
CXXFLAGS=-g -O0 -std=c++11

AR=ar
ARFLAGS=rcs

RM=rm
RMFLAGS=-f

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

OBJ=lexical/pattern.o lexical/tokenizer.o semantic/ast.o semantic/parser.o semantic/error.o

build : $(OBJ)
	$(AR) $(ARFLAGS) libtaurus.a $(OBJ)

clean :
	$(RM) $(RMFLAGS) $(OBJ) libtaurus.a
