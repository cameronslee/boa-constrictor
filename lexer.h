#ifndef LEXER_H
#define LEXER_H

#define INITIAL_CAPACITY 20
#define LEX_BUFF_CAPACITY 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "token.h"
#include "symbol_table.h"

typedef struct {
  char *src;
  size_t src_size;
  char current;
  unsigned int index;
  size_t line_number;
  symtable_T *symtable;
} lexer_T;

lexer_T * init_lexer(char *buffer, size_t buffer_length);
 
void lexer_advance(lexer_T *lexer);

char peek(lexer_T *lexer, int index);

void lexer_skip_whitespace(lexer_T *lexer);

void lexer_skip_newline(lexer_T *lexer);

bool lexer_teardown(lexer_T *lexer);

// returns the index to the symbol table of where the token is 
int lexer_analyze(lexer_T *lexer);

//TODO strip comments
#endif
