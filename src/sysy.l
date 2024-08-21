%option noyywrap
%option nounput
%option noinput

%{

#include <cstdlib>
#include <string>
#include "../type/BaseAST.hpp"
#include "sysy.tab.hpp"

using namespace std;

%}

WhiteSpace    [ \t\n\r]*
LineComment   "//".*
Comment       "/*"([^*]|\*+[^*/])*\*+"/"

Identifier    [a-zA-Z_][a-zA-Z0-9_]*

Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+

%%

{WhiteSpace}    { }
{LineComment}   { }
{Comment}       { }

"int"           { return INT; }
"return"        { return RETURN; }

{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }

"+"             { return ADD; }
"-"             { return SUB; }
"!"             { return LN; }
"*"             { return MUL; }
"/"             { return DIV; }
"%"             { return MOD; }
.               { return yytext[0]; }

%%

