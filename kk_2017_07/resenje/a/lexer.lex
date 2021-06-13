%option noyywrap
%option nounput
%option noinput

%{
#include <iostream>
#include <stdlib.h>
using namespace std;

#include "vektor.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"

%}

/* regularne definicije */
DIGIT  [0-9]
LETTER [a-z]

%%
vector                       { return vector_token; }
print                        { return print_token; }
scaling                      { return scaling_token; }
[-]?{DIGIT}+(\.{DIGIT}+)?    { yylval.num_attr = atof(yytext); return num_token; }
{LETTER}{DIGIT}*            { yylval.s_attr = new string(yytext); return id_vektora_token; }
[=,\[\]();]                  { return *yytext; }
[*+-]                        { return *yytext; }
[\n\t ]                      {  }
.                            { 
                               cerr<<"Leksicka greska: Neprepoznat karakter <"<<*yytext<<endl;
                               exit(EXIT_FAILURE);
                             }

%%

