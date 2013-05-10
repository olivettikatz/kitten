CC=gcc
CFLAGS=-g -O0 --std=c99
LIBS=-lm

AR=ar
ARFLAGS=rcs

RM=rm
RMFLAGS=-f

CFLAGS_X11=$(shell pkg-config --cflags x11 xft imlib2)
LIBS_X11=$(shell pkg-config --libs x11 xft imlib2)

CFLAGS_OPENGL=$(shell pkg-config --cflags gl freetype2)
LIBS_OPENGL=$(shell pkg-config --libs gl freetype2) -lGLU -lglut -lSOIL

OBJ=kernel/backend.o
TEST=test/test_kernel.bin

BACKEND_X11="enable"
BACKEND_OPENGL="enable"

ifeq ($(BACKEND_X11),"enable")
	CFLAGS+=$(CFLAGS_X11)
	LIBS+=$(LIBS_X11)
	OBJ+=kernel/x11/x11.o
else
	CFLAGS+=-DKRD_DISABLE_X11
endif

ifeq ($(BACKEND_OPENGL),"enable")
	CFLAGS+=$(CFLAGS_OPENGL)
	LIBS+=$(LIBS_OPENGL)
	OBJ+=kernel/opengl/opengl.o
else
	CFLAGS+=-DKRD_DISABLE_OPENGL
endif

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.bin : %.o
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ $(LIBS)

build : $(OBJ)
	$(AR) $(ARFLAGS) libkrd.a $(OBJ)

test : $(TEST)

clean :
	$(RM) $(RMFLAGS) $(OBJ) libkrd.a
	for i in $(TEST); do $(RM) $(RMFLAGS) $$i $$(dirname $$i)/$$(basename $$i .bin).o; done
