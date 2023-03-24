CC=gcc
CPP=g++
LEX=flex

all: scanner

clean:
	rm -f *.o lex.yy.c scanner

lex.yy.c: lex.l
	$(LEX) $< -o $@

%.o: %.c
	$(CC) -c $< -o $@

%.o: %.cc
	$(CPP) -c $< -o $@

scanner: main.o lexer.o lex.yy.o
	$(CPP) $^ -o $@

.PHONY: all clean