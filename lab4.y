%token 	NOTOKEN NEWLINE DASH COMMA 

%token <int_val> INT

%token <string_val> CHAR TRACE STATS

%union	{
		char   *string_val;
        int    int_val;
	}


%{
extern "C" 
{
	int yylex();
	void yyerror (char const *s);
}
#define yylex yylex
#include <stdio.h>
// #include "command.h"
%}

%%

//delayed until we fix line 5

goal:
    line1 NEWLINE line2 NEWLINE line3 NEWLINE line4 NEWLINE line5
    ;

line1:
    TRACE {
        printf("Line 1: %s\n", $1);
    }
    ;
    | STATS {
        printf("Line 1: %s\n", $1);
    }
    ;
policy:
    INT {
        printf("i am Line 2, policy: %d\n", $1);
    }

    | INT DASH INT {
        printf("i am Line 2, policy: %d level %d", $1, $3);
    }
    ;

line2:
    //printf("Line 2: \n");
    policy
    | line2 COMMA policy
    ;

line3:
    INT {
        printf("I am Line 3 , i am %d\n",$1);
    }    
    ;

line4:
    INT {
        printf("I am Line 4 , i am %d\n",$1);
    }    
    ;


// fifth line commented for now due to lack some yacc make syntax error


process:
    CHAR COMMA INT COMMA INT {
        printf("%s\n", $1);
       // printf("i am Line 5, name %s, AT %d, ST %d\n", $1, $3, %5);
    }
    ;
line5:
    process NEWLINE
    | line5 process NEWLINE
    ;

%%
void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif