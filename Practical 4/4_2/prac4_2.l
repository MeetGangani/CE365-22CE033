%option noyywrap
%{
    #include <stdio.h>  
%}
%%
charusat {printf("university");}
. {printf("%c", yytext[0]);}
[\t\n] {yyterminate();}
%%

int main(){
    printf("Enter a string: ");
    yylex();
    return 0;
}