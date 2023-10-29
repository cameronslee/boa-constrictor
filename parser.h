#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
  lexer_T *lexer;

} parser_T;

parser_T * parser_init(lexer_T *lexer);

bool parser_teardown(parser_T *parser);

#endif
