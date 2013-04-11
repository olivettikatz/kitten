CXX=g++
CXXFLAGS=-ggdb -O0 -std=c++11

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

build : data.o lexer.o parser.o

test : test.o
	$(CXX) $(CXXFLAGS) test.o lexer.o parser.o data.o -o test.bin

clean : 
	rm -f $(shell echo *.o) test.bin
