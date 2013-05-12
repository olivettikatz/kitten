include ../build/config.mk

OBJ+=lexical/pattern.o lexical/tokenizer.o semantic/ast.o semantic/parser.o semantic/error.o

build : $(OBJ)
	@echo " link libktp.a"
	@$(AR) $(ARFLAGS) libktp.a $(OBJ)

clean :
	@$(RM) $(RMFLAGS) $(OBJ) libktp.a
