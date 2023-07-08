CC=gcc
CPP=g++
LEX=flex
YACC=yacc

CXXFLAGS = -std=c++2b -g
CFLAGS = -g

all: parser

clean:
	rm -f *.o lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h scanner

lex.yy.c: expr.l
	$(LEX) -o $@ $<

y.tab.c y.tab.h: expr.y
	$(YACC) -d $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cc
	$(CPP) $(CXXFLAGS) -c $< -o $@

parser: y.tab.o
	$(CPP) $^ -o $@

.PHONY: all clean