PROG = bits
CC = clang++-12
YACC = bison
LEX = flex
CFLAGS = $(shell llvm-config-12 --cxxflags)
LDFLAGS=$(shell llvm-config-12 --ldflags --system-libs --libs)

$(PROG) : lex.yy.o parser.tab.o ast.o
	$(CC) -o $@ $^ $(LDFLAGS)
lex.yy.o : lex.yy.c  parser.tab.hpp ast.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -Wno-sign-compare -c -o $@ $<
parser.tab.o : parser.tab.cpp parser.tab.hpp ast.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -c -o $@ $<
parser.tab.cpp parser.tab.hpp : parser.ypp
	$(YACC) -d -v $<
lex.yy.c : lexer.lex
	$(LEX) $<
ast.o: ast.cpp ast.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(PROG) *.o *~ *.output *.tab.* *.c
