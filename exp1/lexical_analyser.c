#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char buffer[]) {
  char keywords[32][10] = {
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if","int",
    "long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while"
  };

  int i, flag = 0;
  for (i = 0; i < 32; i++) {
    if (strcmp(keywords[i], buffer) == 0) {
      flag = 1;
      break;
    }
  }
  return flag;
}

int main() {
  int ch;                        
  char buffer[15], operators[] = "+-*/%=";
  FILE * fp;
  int i, j = 0;

  fp = fopen("input.txt", "r");
  if (fp == NULL) {
    printf("error while opening the file\n");
    exit(0);
  }

  while ((ch = fgetc(fp)) != EOF) {

    if (isalnum((unsigned char)ch) || ch == '_') {
      if (j < (int)sizeof(buffer) - 1)
        buffer[j++] = (char)ch;
    } 
    
    else if ((ch == ' ' || ch == '\n' || ch == '\t') && (j != 0)) {
      buffer[j] = '\0';
      j = 0;

      if (isKeyword(buffer) == 1)
        printf("%s is keyword\n", buffer);
      else
        printf("%s is an identifier\n", buffer);
    }

    for (i = 0; i < (int)sizeof(operators) - 1; i++) {
      if (ch == operators[i])
        printf("%c is operator\n", ch);
    }
  }


  if (j != 0) {
    buffer[j] = '\0';
    if (isKeyword(buffer) == 1)
      printf("%s is keyword\n", buffer);
    else
      printf("%s is an identifier\n", buffer);
  }

  fclose(fp);
  return 0;
}