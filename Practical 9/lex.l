%{
#include "y.tab.h"
%}

%%

i     { return I; }
t     { return T; }
e     { return E; }
a     { return A; }
b     { return B; }
else  { return ELSE; }
[ \t\n]  ;  /* Ignore spaces, tabs, and newlines */
.     { printf("Invalid token\n"); exit(1); }

%%

int yywrap() {
    return 1;
}
