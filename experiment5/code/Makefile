CC=gcc
CPP=g++
LEX=flex
YACC=yacc

CXXFLAGS = -std=c++2b -Wformat-truncation= -g
CFLAGS = -g

all: gen

clean:
	rm -f *.o lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h scanner

lex.yy.c: lexer.l
	$(LEX) -o $@ $<

y.tab.c y.tab.h: lex.yy.c grammar.y
	$(YACC) -d grammar.y

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cc
	$(CPP) $(CXXFLAGS) -c $< -o $@

gen: y.tab.o types.o
	$(CPP) $^ -o $@

.PHONY: all clean