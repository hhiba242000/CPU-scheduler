%{

#include <string.h>
#include "y.tab.h"

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
    //yylval.string_val = strdup(yytext);
    return CHAR;
}
[0-9] {
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