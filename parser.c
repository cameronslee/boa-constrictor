#include "parser.h"

parser_T * parser_init(lexer_T *lexer) {
  parser_T *parser = calloc(1, sizeof(parser_T));
  parser->lexer = lexer;

  return parser;
}

bool parser_teardown(parser_T *parser) {
  free(parser);
  parser = NULL;

  return true;
}

  
