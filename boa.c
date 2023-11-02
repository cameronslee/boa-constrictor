/* boa.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 20
#define LEX_BUFF_CAPACITY 128
#define DONE 69420 

/*  ============================= SYMBOL TABLE ==================================
 *
 *
 * 
 * =======================================================================*/

typedef enum {
  IDENTIFIER, // alphanum followed by num || alphanum
  KEYWORD, // keywords loaded into symtable at start
  SEPARATOR, //TODO implement {, }, (, ), ;
  OPERATOR, // TODO implement arith ops
  LITERAL, // TODO for now, just a sequence of digits
  NONE,
} token_name_T;

typedef struct {
  token_name_T type;
  /* Attribute value usage
  // literals are stored as they are.
  // identifiers stored as the index into the sym table
  */
  char *value; // attribute value
} token_T;

typedef struct {
  size_t size;
  size_t capacity;
  int last_pos; // last used pos in symtable
  token_T *table;
} symtable_T;

// returns the index of the lexeme in the symtable. 
// returns 0 if it does not exist
int lookup(symtable_T *t, char lexeme[]) {
  int i;
  for (i = t->last_pos; i > 0; i--) {
    if (strcmp(t->table[i].value, lexeme) == 0) return i;
  }
  return 0;
}

// creates a token and adds it to the symtable
// returns the position of entry of the lexeme
int insert(symtable_T *t, char lexeme[], token_name_T type) {
  int len;
  size_t new_size = 0;
  len = strlen(lexeme);
  if ((unsigned long)(t->last_pos + 1) >= t->capacity) {
    // TODO for now, just double the size of the table
    new_size = (t->capacity* 2) * sizeof(token_T);
    t = realloc(t, new_size); 
  }

  t->last_pos += 1;
  t->size += 1;
  t->table[t->last_pos].value = malloc(sizeof(char) * len);
  t->table[t->last_pos].type = type;

  strncpy(t->table[t->last_pos].value, lexeme, len);

  return t->last_pos;
}

// init and load with keywords
symtable_T *init_symtable(size_t initial_size) {
  symtable_T *symtable = malloc(sizeof(symtable_T));
  symtable->table = malloc(sizeof(token_T) * initial_size);
  symtable->last_pos = 0;
  symtable->size = 0;
  symtable->capacity= initial_size;
  
  //load keywords
  insert(symtable, "int", KEYWORD);
  return symtable;
}
const char * get_token_name(token_name_T type) {
  switch(type) {
    case IDENTIFIER: return "IDENTIFIER";
    case KEYWORD: return "KEYWORD";
    case SEPARATOR: return "SEPARATOR";
    case OPERATOR: return "OPERATOR";
    case LITERAL: return "LITERAL";
    case NONE: return "NONE";
  }

  return "ERROR";
}

/* ================================ LEXER =================================
 *
 *
 * 
 * =======================================================================*/
typedef struct {
  char *src;
  size_t src_size;
  char current;
  unsigned int index;
  size_t line_number;
  symtable_T *symtable;
} lexer_T;

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
// returns the index to the symbol table of where the token is 
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

/* ================================ PARSER =================================
 *
 *
 * 
 * =======================================================================*/
typedef struct {
  lexer_T *lexer;
  token_T *tokens;
} parser_T;

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

/* ================================ DRIVER =================================
 *
 *
 * 
 * =======================================================================*/
size_t get_file_size(FILE *f) {
	size_t file_size;
	if( fseek(f, 0, SEEK_END) != 0 ) exit(EXIT_FAILURE); 

	file_size = ftell(f);
	rewind(f);
	return file_size;
}

void read_file(FILE * f, char *buffer, size_t file_size) {
	if (file_size == 1) exit(EXIT_FAILURE);

	fread(buffer, 1, file_size, f);
	if (buffer == NULL) exit(EXIT_FAILURE);

	fclose(f); 
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("%s\n", "usage: boa <filename>");
    exit(1);
  }

  FILE *f;
  if(!(f = fopen(argv[1], "r"))) {
    printf("%s\n", "error: file not found");
    exit(1);
  }

  printf("%s\n", "Hello Boa Constrictor");

  // File buffer setup
  size_t file_size = get_file_size(f);
	char *buffer = malloc(file_size * sizeof(char));

  read_file(f, buffer, file_size);

  printf("%s\n", buffer); 

  /* Lex entry point */
  printf("%s\n", "Enter: Lexer");
  lexer_T *lexer = init_lexer(buffer, file_size);

  /* Print symbol table */
  int p = lexer_analyze(lexer);
  while (p != DONE) {
    printf("Token: %s\nToken Type: %s\n", lexer->symtable->table[p].value, get_token_name(lexer->symtable->table[p].type));
  p = lexer_analyze(lexer);
  }

  free(buffer);

  return 0;
}