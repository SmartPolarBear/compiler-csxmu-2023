CC=gcc
CPP=g++
LEX=flex

CXXFLAGS = -std=c++2b -g
CFLAGS = -g

all: scanner

clean:
	rm -f *.o lex.yy.c scanner

lex.yy.c: lex.l
	$(LEX) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cc
	$(CPP) $(CXXFLAGS) -c $< -o $@

scanner: main.o lexer.o lex.yy.o
	$(CPP) $^ -o $@

.PHONY: all clean