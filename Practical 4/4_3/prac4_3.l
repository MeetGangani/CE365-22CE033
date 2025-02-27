%option noyywrap
%{
    #include <stdio.h>
    int l=0, w=0, c=0;
%}
%%
[\n] {l++;}
[ \t]+ {c+=yyleng;}
[^ \t\n]+ {w++; c+=yyleng;}
. ;
%%
int main(){
    printf("Enter a string: ");
    yylex();
    printf("\nCharacters: %d\nWords: %d\nLines: %d\n", c, w, l);
    return 0;
}