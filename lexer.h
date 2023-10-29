#ifndef LEXER_H
#define LEXER_H

#define INITIAL_CAPACITY 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
} token_T;

typedef struct {
  char *src;
  size_t src_size;
  char current;
  unsigned int index;
  token_T *tokens;
} lexer_T;

lexer_T * init_lexer(char *buffer, size_t buffer_length);
 
void lexer_advance(lexer_T *lexer);

char peek(lexer_T *lexer, int index);

void lexer_skip_whitespace(lexer_T *lexer);

bool lexer_teardown(lexer_T *lexer);

token_T * lexer_parse(lexer_T *lexer);
#endif
