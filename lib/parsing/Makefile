include ../config.mk

OBJ+=lexical/pattern.o lexical/tokenizer.o semantic/ast.o semantic/parser.o semantic/error.o

build : $(OBJ)
	$(AR) $(ARFLAGS) libparsing.a $(OBJ)

clean :
	$(RM) $(RMFLAGS) $(OBJ) libparsing.a
