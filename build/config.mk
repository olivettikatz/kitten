# PROGARMS
CC=gcc
CFLAGS=-ggdb -O0 --std=gnu99

CXX=g++
CXXFLAGS=-ggdb -O0 -std=c++11

AR=ar
ARFLAGS=rcs

RM=rm
RMFLAGS=-f --

# LIBRARY FLAGS
CFLAGS_MATH=
CFLAGS_X11=$(shell pkg-config --cflags x11 xft imlib2)
CFLAGS_OPENGL=$(shell pkg-config --cflags gl freetype2)

# LIBRARY INCLUDES
LIBS_MATH=-lm
LIBS_X11=$(shell pkg-config --libs x11 xft imlib2)
LIBS_OPENGL=$(shell pkg-config --libs gl freetype2) -lGLU -lglut -lSOIL

# AUTOMATIC TARGETS
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.bin : %.c
	$(CC) $(CFLAGS) $< $(OBJ) -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.bin : %.cpp
	$(CXX) $(CXXFLAGS) $< $(OBJ) -o $@

# CONFIGURATION
PWD=$(shell pwd)
OBJ=
TEST=