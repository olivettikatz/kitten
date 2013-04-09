CC=gcc
CFLAGS=-g -O2 --std=c99

RM=rm
RMFLAGS=-f

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

OBJ=data/encoding.o data/error.o data/token.o parsing/error.o parsing/lex.o parsing/symbols.o parsing/iasm.o parsing/parse.o

build : $(OBJ)

testing/test_lexer.bin : $(OBJ) testing/test_lexer.o
	$(CC) $(CFLAGS) $^ -o $@

testing/test_iasm.bin : $(OBJ) testing/test_iasm.o
	$(CC) $(CFLAGS) $^ -o $@

testing : testing/test_lexer.bin testing/test_iasm.bin

clean : 
	$(RM) $(RMFLAGS) $(OBJ) $(shell echo testing/*.o testing/*.bin)
