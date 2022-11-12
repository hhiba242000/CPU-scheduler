%{

#include <string.h>
#include "y.tab.h"
// extern double yylval;
void yyerror(char *s);
int yylex();
%}

%%

\n 	{
        printf("\n");
		return NEWLINE;
	}

trace {
    printf("trace ");
    return TRACE;
    }

stats {
    printf("stats ");
    return STATS;
    }

\-    { 
    printf("dash ");
    return DASH;
    }

, {
    printf("comma ");
    return COMMA;
}

[a-zA-z] {
    printf("letter ");
    yylval.string_val = strdup(yytext);
    return CHAR;
}
[0-9]+ {
    yylval.string_val = strdup(yytext);
    printf("int ");
    return INT;
    }

[ \t] 	{
		/* Discard spaces and tabs */
     	}


.	{
		/* Invalid character in input */
		return NOTOKEN;
	}

%%
int yywrap(void) {return 1;}