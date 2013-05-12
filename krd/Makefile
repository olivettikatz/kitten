include config.mk

OBJ+=kernel/backend.o
TEST+=test/test_kernel.bin

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

build : $(OBJ)
	$(AR) $(ARFLAGS) libkrd.a $(OBJ)

test : $(TEST)

clean :
	$(RM) $(RMFLAGS) $(OBJ) $(TEST) libkrd.a
