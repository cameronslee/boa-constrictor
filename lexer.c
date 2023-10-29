#include "lexer.h"

lexer_T * init_lexer(char *buffer, size_t buffer_length) {
  lexer_T * lexer = calloc(1, sizeof(lexer_T));
  lexer->src = buffer;
  lexer->src_size = buffer_length;
  lexer->index = 0;
  lexer->current = lexer->src[lexer->index];
  lexer->tokens= calloc(1, sizeof(lexer_T));

  return lexer;
}

void lexer_advance(lexer_T *lexer) {
  if (lexer->index < lexer->src_size && lexer->current != '\0') {
    lexer->index++;
    lexer->current = lexer->src[lexer->index];
  }
}

char peek(lexer_T *lexer, int offset) {
  if (lexer->src_size < (lexer->index + offset)) {
    perror ("error: offset in peek() out of bounds");
    exit(EXIT_FAILURE);
  }

  return lexer->current + offset;
}

void lexer_skip_whitespace(lexer_T *lexer) {
  while (lexer->current == ' ' || lexer->current == '\n' || lexer->current == '\t') {
    lexer_advance(lexer);
  }
}

token_T * lexer_parse(lexer_T lexer) {

}

bool lexer_teardown(lexer_T *lexer) {
  free(lexer->tokens);
  free(lexer);
  lexer = NULL;

  return true;
}
