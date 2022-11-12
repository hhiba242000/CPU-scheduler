%{
void yyerror (char *s);
int yylex();
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>



// #define yylex yylex

// #include "command.h"
%}
%union	{
		char   string_val;
	}
%token <string_val> CHAR TRACE STATS INT
%token 	NOTOKEN NEWLINE DASH COMMA


%%

// //delayed until we fix line 5
// goal: 
//     line1
//     ;
// goal:
//     line1 NEWLINE line2 NEWLINE line3 NEWLINE line4 NEWLINE line5
//     ;

// line1:
//     TRACE {
//         //printf("Line 1: %s\n", $1);
//         printf("JJ");
//     }
//     | STATS {
//         printf("Line 1: %s\n", $1);
//     }
//     ;
// policy:
//     INT {
//         printf("i am Line 2, policy: %s\n", $1);
//     }

//     | INT DASH INT {
//         printf("i am Line 2, policy: %s level %s", $1, $3);
//     }
//     ;

// line2:
//     //printf("Line 2: \n");
//     policy
//     | line2 COMMA policy
//     ;

line3:
    INT {
        printf("I am Line 3 , i am \n");
    }    
    ;

// line4:
//     INT {
//         printf("I am Line 4 , i am %s\n",$1);
//     }    
//     ;


// fifth line commented for now due to lack some yacc make syntax error


/* process:
    CHAR COMMA INT COMMA INT {
        //printf("%s\t%s\t%s", $1, $3, $5);
        printf("i am Line 5, name %s, AT %s, ST %s\n", $1, $3, $5);
    }
    ;
line5:
    process NEWLINE
    | line5 process NEWLINE
    ; */

%%
int main (void) {

	return yyparse ( );
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 
// void
// yyerror(const char * s)
// {
// 	fprintf(stderr,"%s", s);
// }

// #if 0
// main()
// {
// 	yyparse();
// }
// #endif