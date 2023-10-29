#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

size_t get_file_size(FILE *f)
{
	size_t file_size;
	if( fseek(f, 0, SEEK_END) != 0 ) exit(EXIT_FAILURE); 

	file_size = ftell(f);
	rewind(f);
	return file_size;
}

void read_file(FILE * f, char *buffer, size_t file_size) {
	if (file_size == 1) exit(EXIT_FAILURE);

	fread(buffer, 1, file_size, f);
	if (buffer == NULL) exit(EXIT_FAILURE);

	fclose(f); 
}

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

  printf("%s\n", "Hello Boa Constrictor");

  // File buffer setup
  size_t file_size = get_file_size(f);
	char *buffer = malloc(file_size * sizeof(char));

  read_file(f, buffer, file_size);

  printf("%s\n", buffer); 

  /* Lex entry point */
  lexer_T *lexer = init_lexer(buffer, file_size);

  free(buffer);

  return 0;
}
