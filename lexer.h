#ifndef LEXER_H
#define LEXER_H

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


TokenArray *lexer(FILE *f);
long get_file_size(FILE *f);
void add_token(TokenArray *tokens, Token token);

// this should check if we can read something in
//returns the index we can continue checking at
//returns -1 if fails
int peek(char *buffer, int index);
Token *generate_token(char *lexeme, int size);
  

#endif
