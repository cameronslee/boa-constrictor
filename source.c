#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("%s\n", "usage: boa <filename>");
    exit(1);
  }

  FILE *f;
  if(!(f = fopen(argv[1], "r"))) {
    printf("%s\n", "error: file not found");
    exit(1);
  }

  TokenArray *tokens = lexer(f);


  printf("NUM TOKENS %zu\n", tokens->size); 
  printf("CAPACITY %zu\n", tokens->capacity); 
  Token t = tokens->array[0];

  for(int i = 0; t.value[i] != '\0'; i++){
    printf("%c", t.value[i]);
  }



  printf("\n%s\n", "Hello Boa Constrictor");

  return 0;
}
