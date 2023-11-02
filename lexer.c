#include "lexer.h"

lexer_T * init_lexer(char *buffer, size_t buffer_length) {
  lexer_T * lexer = calloc(1, sizeof(lexer_T));
  lexer->src = buffer;
  lexer->src_size = buffer_length;
  lexer->index = 0;
  lexer->line_number = 1;
  lexer->current = lexer->src[lexer->index];

  lexer->symtable = init_symtable(INITIAL_CAPACITY);

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
  while (lexer->current == ' ' || lexer->current == '\t') {
    lexer_advance(lexer);
  }
}

void lexer_skip_newline(lexer_T *lexer) {
  while (lexer->current == '\n') {
    lexer_advance(lexer);
  }
}

// designed to be called by the parser
int lexer_analyze(lexer_T *lexer) {
  char lex_buff[LEX_BUFF_CAPACITY];
  int p, b = 0 ;
  while (1) {
    if (lexer->current == ' ' || lexer->current == '\t') {
      lexer_skip_whitespace(lexer);
    }
    else if (lexer->current == '\n') {
      lexer_skip_newline(lexer);
    }
    else if (isalpha(lexer->current)) {
      while (isalnum(lexer->current)) {
        lex_buff[b] = lexer->current;
        lexer_advance(lexer);
        b += 1;
        if (b >= LEX_BUFF_CAPACITY) {
          perror("error: lexer buffer capacity exceeded");
          exit(1);
        }
      }
      lex_buff[b] = '\0';
      p = lookup(lexer->symtable, lex_buff);
      if (p == 0) {
        p = insert(lexer->symtable, lex_buff, IDENTIFIER);
      }
      return p;
    }
    // if this project gets to the point where its reading files this big im legendary
    else if (lexer->current == '\0') return DONE;
    // RANDOM
    else {
      /*
      while (isalnum(lexer->current)) {
        lex_buff[b] = lexer->current;
        lexer_advance(lexer);
        b += 1;
        if (b >= LEX_BUFF_CAPACITY) {
          perror("error: lexer buffer capacity exceeded");
          exit(1);
        }
      }
      lex_buff[b] = '\0';
      p = lookup(lexer->symtable, lex_buff);
      if (p == 0) {
        p = insert(lexer->symtable, lex_buff, NONE);
      }
      return p;
    */
    }
  }

  return -1; //fook
}

bool lexer_teardown(lexer_T *lexer) {
  free(lexer);
  lexer = NULL;

  return true;
}
