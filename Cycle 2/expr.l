%{
#include "y.tab.h"
%}

%%
[0-9]+      { yylval.num = atoi(yytext); return NUM; }
[+\-*/()]   { return yytext[0]; }
[ \t\n]+    ; // ignore whitespace
.           { printf("Invalid character: %s\n", yytext); }
%%
