%{
	#include <stdio.h>
	#include <stdlib.h>
	#define YYSTYPE double
	int yylex(void);
%}

%token DIGIT ID NL
%left '-' '+'
%left '*' '/'
%token '='

%%
stmt : ID '=' exp NL {
	printf("Valid expression\n");
	exit(0);
}
| exp NL {
	printf("Valid expression\n");
	exit(0);
};
exp :exp '+' exp
| exp '-' exp
| exp '*' exp
| exp '/' exp
|'(' exp ')'
| ID
| DIGIT;
%%

int yyerror(char *msg) {
	printf("Invalid string\n");
	return 0;
}


int main () {
	printf("Enter the exp: ");
	yyparse();
	return 0;
}



/*
lex lexc.l
yacc -d yac.y
gcc lex.yy.c y.tab.c -lfl

./a.out
Enter the exp: a=b+c
Valid expression

./a.out
Enter the exp: a+b=c
Invalid string
*/