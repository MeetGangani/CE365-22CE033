%{
#include <stdio.h>
#include <stdlib.h>
%}

%token I T E A B ELSE

%%

S  : I EXPR T S S1 | A ;
S1 : ELSE S | /* ε (empty production) */ ;
EXPR  : B ;

%%

int yyerror(char *msg) {
    printf("Invalid string\n");
    exit(1);
}

int main() {
    printf("Enter the input string: ");
    yyparse();
    printf("Valid string\n");
    return 0;
}
