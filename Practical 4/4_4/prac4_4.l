%option noyywrap
%{
    #include <stdio.h>
    int d=0, lc=0, uc=0, sc=0, l=0;  
%}
%%
[0-9] {d++;l++;}
[a-z] {lc++;l++;}
[A-Z] {uc++;l++;}
[*,;#$@] {sc++;l++;}
[\n] {
    if( d > 0 && lc >0 && uc > 0 && l >= 9 && l <= 15 && sc>0){
        printf("Valid\n");
    }
    else{
        printf("Invalid\n");
    }
    yyterminate();
} 
. { printf("Invalid\n"); yyterminate();  }
%%
int main(){
    printf("Enter a Password : ");
    yylex();
    return 0;
}