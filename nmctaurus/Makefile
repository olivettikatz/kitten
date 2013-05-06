CXX=g++
CXXFLAGS=-g -O0 -std=c++11
CXXLIBS=-ltaurus

RM=rm
RMFLAGS=-f

OBJ=parsing.o
TEST=test_parsing.bin

TAURUS=taurus
CXXFLAGS+=-I$(TAURUS)
CXXLIBS+=-L$(TAURUS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.bin : %.cpp
	$(CXX) $(CXXFLAGS) $(OBJ) $< -o $@ $(CXXLIBS)

build : $(OBJ)

test : $(TEST)

clean :
	$(RM) $(RMFLAGS) $(OBJ) $(TEST)
