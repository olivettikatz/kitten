# PROGARMS
CC=gcc
CFLAGS=-ggdb -O0 --std=gnu99 -I../build -L../build

CXX=g++
CXXFLAGS=-ggdb -O0 -std=c++11 -I../build -L../build

AR=ar
ARFLAGS=rcs

RM=rm
RMFLAGS=-rf --

CP=cp
CPFLAGS=-rf --

MKDIR=mkdir
MKDIRFLAGS=-p

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
	@echo " building $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.bin : %.c
	@echo " building $<"
	@$(CC) $(CFLAGS) $< $(OBJ) -o $@
	@echo " linking $@"

%.o : %.cpp
	@echo " building $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

%.bin : %.cpp
	@echo " building $<"
	@$(CXX) $(CXXFLAGS) $< $(OBJ) -o $@
	@echo " linking $@"

# CONFIGURATION
PWD=$(shell pwd)
OBJ=
TEST=
