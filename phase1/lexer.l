%{
/* All C++ code must go here or after the final %% */

# include <iostream>

using namespace std;

%}

%option nounput noyywrap yylineno
%%

"auto"                  { cout << "keyword " << yytext << endl;}

[0-9]+                  { cout << "integer " << yytext << endl;}

[ \t\n\v\f]+            { /* ignored */ }

.                       { /* ignored */ }
%%

/* More C++ code */

int main()
{
    while(yylex())
        continue;
}