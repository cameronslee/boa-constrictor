#include "symbol_table.h"

int lookup(symtable_T *t, char *lexeme) {
  int i;
  for (i = t->last_pos; i >= 0; i--) {
    if (strcmp(t->table[i].value, lexeme) == 0) return i;
  }
  return 0;
}

int insert(symtable_T *t, char *lexeme, token_name_T type) {
  int len;
  size_t new_size;
  len = strlen(lexeme);
  if (t->last_pos + 1 >= t->size) {
    // TODO for now, just double the size of the table
    new_size = (t->size * 2) * sizeof(token_T);
    t = realloc(t, new_size); 
  }

  t->last_pos += 1;
  t->table[t->last_pos].type = type;
  t->table[t->last_pos].value = lexeme;

  strncpy(t->table[t->last_pos].value, lexeme, len);
  return t->last_pos;
}

symtable_T *init_symtable(size_t initial_size) {
  symtable_T *symtable = malloc(sizeof(symtable_T));
  symtable->table = malloc(sizeof(token_T) * initial_size);
  symtable->last_pos = 0;
  symtable->size = initial_size;
  
  //load keywords
  char *foo;
  foo = "int";
  insert(symtable, foo, KEYWORD);

  return symtable;
}
