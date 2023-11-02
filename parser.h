#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
  lexer_T *lexer;
  token_T *tokens;
} parser_T;

parser_T * parser_init(lexer_T *lexer);

bool parser_teardown(parser_T *parser);

//called over and over until the full symbol table has been formed
void get_next_token(parser_T *parser);

#endif
