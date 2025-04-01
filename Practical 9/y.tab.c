#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

extern int yylex();
extern char* yytext;
int yyerror(char *msg);

/* Simple parser for the given grammar:
 * S  : I EXPR T S S1 | A ;
 * S1 : ELSE S | empty ;
 * EXPR : B ;
 */
int yyparse() {
    int token;
    int state = 0;
    
    while((token = yylex()) != 0) {
        switch(state) {
            case 0: /* Initial state */
                if(token == I) {
                    state = 1; /* Got 'I', expecting EXPR */
                } else if(token == A) {
                    state = 10; /* Got 'A', S → A production complete */
                    return 0;
                } else {
                    yyerror("Syntax error: Expected 'i' or 'a'");
                    return 1;
                }
                break;
                
            case 1: /* After seeing I, expecting EXPR (which is B) */
                if(token == B) {
                    state = 2; /* Got 'B', expecting T */
                } else {
                    yyerror("Syntax error: Expected 'b'");
                    return 1;
                }
                break;
                
            case 2: /* After seeing I EXPR, expecting T */
                if(token == T) {
                    state = 3; /* Got 'T', expecting S */
                } else {
                    yyerror("Syntax error: Expected 't'");
                    return 1;
                }
                break;
                
            case 3: /* After seeing I EXPR T, expecting S */
                /* Recursive call to parse S */
                yyparse();
                state = 4; /* After S, expecting S1 */
                break;
                
            case 4: /* After seeing I EXPR T S, expecting S1 */
                if(token == ELSE) {
                    state = 5; /* Got 'ELSE', expecting S */
                } else {
                    /* S1 can be empty, so we're done */
                    state = 6;
                }
                break;
                
            case 5: /* After seeing I EXPR T S ELSE, expecting S */
                /* Recursive call to parse S */
                yyparse();
                state = 6; /* After S1 → ELSE S, we're done */
                break;
                
            case 6: /* Final state after seeing I EXPR T S S1 */
                return 0;
                
            default:
                yyerror("Internal parser error");
                return 1;
        }
    }
    
    /* End of input */
    if(state == 0) {
        yyerror("Syntax error: Empty input");
        return 1;
    } else if(state == 10) {
        /* We recognized S → A */
        return 0;
    } else {
        yyerror("Syntax error: Incomplete input");
        return 1;
    }
}

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