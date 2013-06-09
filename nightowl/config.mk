# PROGARMS
CC=gcc
CFLAGS=-ggdb -O0 --std=gnu99 -I. -L. -I.. -L.. -I../build -L../build

CXX=g++
CXXFLAGS=-ggdb -O0 -std=c++11 -I. -L. -I.. -L.. -I../build -L../build -I../../build -L../../build

AR=ar
ARFLAGS=rcs

RM=rm
RMFLAGS=-rf --

CP=cp
CPFLAGS=-rf --

MKDIR=mkdir
MKDIRFLAGS=-p

MAKE=make
MAKEFLAGS=

INSTALL=install
INSTALLFLAGS=

# LIBRARY FLAGS
CFLAGS_MATH=
CFLAGS_X11=$(shell pkg-config --cflags x11 xft imlib2)
CFLAGS_OPENGL=$(shell pkg-config --cflags gl freetype2)

# LIBRARY INCLUDES
LIBS_MATH=-lm
LIBS_X11=$(shell pkg-config --libs x11 xft imlib2)
LIBS_OPENGL=$(shell pkg-config --libs gl freetype2) -lGLU -lglut -lSOIL -lm

# AUTOMATIC TARGETS
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.bin : %.c
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ $(LIBS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.bin : %.cpp
	$(CXX) $(CXXFLAGS) $< $(OBJ) -o $@ $(LIBS)

# CONFIGURATION
PWD=$(shell pwd)
OBJ=
TEST=
LIBS=

# INSTALLATION PATHS
PATH_INCLUDE=/usr/include/kitten
PATH_LIB=/usr/lib/kitten
