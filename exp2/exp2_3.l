%{
    #include <stdio.h>
    #include <string.h>

    #define MAX_LEN 1024
    char converted[MAX_LEN] = ""; 
    #define ABC_TOKEN 1
    #define CHAR_TOKEN 2
%}

%%

abc {
    return ABC_TOKEN;
}

. {
    return CHAR_TOKEN;
}

\n {
    return 0;
}

%%

int main() {
    int token;
    printf("Enter the String: ");

    while ((token = yylex()) != 0) {
        if (token == ABC_TOKEN) {
            strcat(converted, "ABC");
        } else if (token == CHAR_TOKEN) {
            strcat(converted, yytext);
        }
    }

    printf("\nConverted string: %s\n", converted);
    return 0;
}

int yywrap() {
    return 1;
}


/*
lex exp2_3.l
gcc lex.yy.c
./a.out

Enter the String: abcg

Converted string: ABCg
*/