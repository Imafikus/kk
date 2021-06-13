PROG = geometry
CC = clang++-12
YACC = bison
lex = flex
CFLAGS = $(shell llvm-config-12 --cxxflags)
LDFLAGS=$(shell llvm-config-12 --ldflags --system-libs --libs)

$(PROG) : lex.yy.o parser.tab.o vektor.o ast.o
	$(CC) -o $@ $^ $(LDFLAGS)
lex.yy.o : lex.yy.c parser.tab.hpp vektor.hpp ast.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -Wno-sign-compare -c -o $@ $<
parser.tab.o : parser.tab.cpp vektor.hpp ast.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -c -o $@ $<
parser.tab.cpp parser.tab.hpp : parser.ypp
	$(YACC) -d -v $<
lex.yy.c : lexer.lex
	$(LEX) $<
vektor.o: vektor.cpp vektor.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -c -o $@ $<
ast.o: ast.cpp ast.hpp vektor.hpp
	$(CC) -Wno-unknown-warning-option $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(PROG) *.o *~ *.output *.tab.* *.c
