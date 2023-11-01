#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "token.h"

typedef struct {
  token_T *table;
  size_t size;
  unsigned int last_pos; // last used pos in symtable
} symtable_T;

// returns the index of the lexeme in the symtable. 
// returns 0 if it does not exist
int lookup(symtable_T *table, char *lexeme); 

// creates a token and adds it to the symtable
// returns the position of entry of the lexeme
int insert(symtable_T *table, char *lexeme, token_name_T type);  

// init and load with keywords
symtable_T * init_symtable(size_t initial_size);


#endif
