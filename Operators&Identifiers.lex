%{
int operator_count = 0;
int identifier_count = 0;
%}

%%
"+"|"-"|"*"|"/" {printf("%s is an operator\n", yytext); operator_count++;}
[a-zA-Z][a-zA-Z0-9_]* {printf("%s is an identifier\n", yytext); identifier_count++;}
[ \t\n] ; /* skip whitespace and newline */
. ; /* skip any other character */
%%

int main()
{
    printf("Enter an arithmetic expression: ");
    yylex();
    printf("\nNumber of operators: %d\n", operator_count);
    printf("Number of identifiers: %d\n", identifier_count);
    return 0;
}

int yywrap() { return 1; }

flex sample.flex
gcc lex.yy.c
a.exe
