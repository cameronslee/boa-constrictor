#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 20
typedef enum {
  IDENTIFIER,
  KEYWORD,
  SEPARATOR,
  OPERATOR,
  LITERAL,
  COMMENT
} TokenName;

typedef struct {
  TokenName type;
  char *value;
} Token; 

typedef struct {
  Token *array;
  size_t size;
  size_t capacity;
} TokenArray;

/* excepts file stream which is already opened */
long get_file_size(FILE *f)
{
	long file_size;
	if( fseek(f, 0, SEEK_END) != 0 ) exit(EXIT_FAILURE); 

	file_size = ftell(f);
	rewind(f);
	return file_size;
}

void add_token(TokenArray* arr, Token *token) {
  if (arr->size >= arr->capacity) {
    arr->size *= 2;
    arr->array = realloc(arr->array, arr->capacity * sizeof(Token));
  }
  arr->array[arr->size] = *token;
  arr->size += 1;
}

Token *generate_token(char *lexeme, int size) {
  Token *new_token = malloc(sizeof(Token));
  new_token->type = KEYWORD;
  new_token->value = lexeme;
  return new_token;
}

int peek(char *buffer, int index) {
  int lexeme_end = index;
  while (buffer[lexeme_end] != ' ' && buffer[lexeme_end] != '\t' && buffer[lexeme_end] != '\n') {
    lexeme_end++;
  }

  return lexeme_end == index ? -1 : lexeme_end;
}

TokenArray *lexer(FILE *f) {
	long file_size;
  int index = 0;
  TokenArray *tokens = malloc(sizeof(TokenArray));
  
  //init token array
  tokens->array = malloc(INITIAL_CAPACITY * sizeof(Token));
  tokens->size = 0;
  tokens->capacity = INITIAL_CAPACITY;

  file_size = get_file_size(f);
  if (file_size == 1) exit(EXIT_FAILURE);

  char *buffer = malloc(file_size * sizeof(char));
  fread(buffer, 1, file_size, f);
  if (buffer == NULL) exit(EXIT_FAILURE);

  fclose(f);

  while(buffer[index] != '\0') {
    //Move past whitespace
    if (buffer[index] == ' ' || buffer[index] == '\t' || buffer[index] == '\n') index++;
    else {
      int lexeme_end = peek(buffer, index);
      if (lexeme_end != -1) {
        int lexeme_size = lexeme_end - index;
        char *lex_buff = malloc(sizeof(char) * lexeme_size+1);
        memcpy(lex_buff, &buffer[index], lexeme_size);
        lex_buff[lexeme_size] = '\0';

        Token *new_token = malloc(sizeof(Token));

        new_token = generate_token(lex_buff, lexeme_size);
        add_token(tokens, new_token);

        printf("NEW TOKEN !!! %s\n", tokens->array[0].value);
        index = lexeme_end;

        free(lex_buff);
        free(new_token);
      }
    }
  }

  free(buffer);
  return tokens;
}


