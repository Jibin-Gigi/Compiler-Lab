%{
	#include <stdio.h>
	int v = 0;
	int c = 0;
%}

%%
[aeiouAEIOU] {
	v++;
}

[a-zA-Z] {
	c++;
}
%%

int yywrap() {
	return 1;
}

int main() {
	printf("Enter the string of vowels and consonants:\n");
	yylex();
	printf("Number of vowels are: %d\n", v);
	printf("Number of consonants are: %d\n", c);
	return 0;
}

/*
lex exp2_4.l
gcc lex.yy.c
./a.out


Enter the string of vowels and consonants(Press Ctrl+D to end input):
acer

Number of vowels are: 2
Number of consonants are: 2

*/