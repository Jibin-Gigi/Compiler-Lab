%{
	#include<stdio.h>
	int flag=0;
%}

%token NUMBER
%left '+' '-'
%left '*' '/' '%'
%left '(' ')'


%%
ArithmeticExpression: E{
	printf("\nResult=%d\n", $$);
	return 0;
};
E:E'+'E {$$=$1+$3;}
|E'-'E {$$=$1-$3;}
|E'*'E {$$=$1*$3;}
|E'/'E {$$=$1/$3;}
|E'%'E {$$=$1%$3;}
|'('E')' {$$=$2;}
| NUMBER {$$=$1;}
;
%%


void main() {
	printf("\nEnter Any Arithmetic Expression which can have operations Addition, Subtraction,Multiplication, Division, Modulus and Round brackets :\n");
	yyparse();
	if(flag==0)
		printf("\nEntered arithmetic expression is Valid\n\n");
}

void yyerror() {
	printf("\nEntered arithmetic expression is Invalid\n\n");
	flag=1;
}


/*
lex lexc.l
yacc –d yac.y
gcc lex.yy.c y.tab.c –lfl

./a.out
Enter Any Arithmetic Expression which can have operations Addition, Subtraction,Multiplication, Division, Modulus and Round brackets :
10-2
Result=8
Entered arithmetic expression is Valid
*/