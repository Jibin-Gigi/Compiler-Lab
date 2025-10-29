%{
#include <stdio.h>

int total = 0;
int keywords = 0;
int identifiers = 0;
int operators = 0;
int numbers = 0;
int literals = 0;

%}

%%

if|else|while|int|switch|for|char {
  printf("%s is a keyword\n", yytext);
  keywords++; 
  total++;
}

["][A-Za-z0-9_]*["] {
  printf("%s is a literal\n", yytext);
  literals++; 
  total++;
}

[A-Za-z_][A-Za-z0-9_]* {
  printf("%s is an identifier\n", yytext);
  identifiers++; 
  total++;
}

"="|"+"|"-"|"*"|"/"|"<"|">" {
  printf("%s is an operator\n", yytext);
  operators++; 
  total++;
}

[0-9]+ {
  printf("%s is a number\n", yytext);
  numbers++; 
  total++;
}

[ \t\n]+ { }


. { printf("unrecognized character: %s\n", yytext); }

%%

int yywrap() {

  printf("\nTotals:\n");
  printf("  keywords:    %d\n", keywords);
  printf("  identifiers: %d\n", identifiers);
  printf("  operators:   %d\n", operators);
  printf("  numbers:     %d\n", numbers);
  printf("  literals:    %d\n", literals);
  printf("  total tokens:%d\n", total);
  return 1; 
}

int main(void) {
  printf("Enter the input text (Ctrl+D to end input):\n");
  yylex();
  return 0;
}


/*
lex exp2_1.l
gcc lex.yy.c
./a.out 
Enter the input text: c=a+b
c is an  identifier
= is an operator
a is an  identifier
+ is an operator
b is an  identifier
*/