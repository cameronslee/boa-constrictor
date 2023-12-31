/* boa.c
 * toy compiler & language
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 100 // initial capacity of symbol table
#define LEX_BUFF_CAPACITY 128 // lexer buffer capacity
#define MAX_OPERATOR_LEN 2 // language specific rule

#define MAX_CHILDREN 100 //FIXME TEMPORARY SIZE FOR AST!!!!!!!!!!!!!!!!!!!!!

#define DONE 69420 // lexer done reading file buffer
#define ERROR 42069

/*  ============================= TOKEN ====================================
 * TODO could consider refactoring to start values at 255
 * For single char tokens to use their own ascii value
 * =======================================================================*/

typedef enum {
  SEMI, 
  COMMA,
  OCB,
  CCB,
  OP,
  CP,
  OS,
  CS,
  DOT,
  EQ,
  ADD,
  SUB,
  MUL,
  GT,
  LT,
  ASSIGN,
  INT,
  STR,
  BOOL,
  PRINT,
  BIND,
  CONSTRICT,
  FN,
  RETURN,

  PROGRAM, // this is used to denote the root node of th4e AST

  IDENTIFIER,
  LITERAL,
  VAR_DECL,
} token_name_T;

const char * get_token_name(token_name_T type) {
  switch(type) {
    case SEMI: return "SEMI";
    case COMMA: return "COMMA";
    case OCB: return "OCB";
    case CCB: return "CCB";
    case OP: return "OP";
    case CP: return "CP";
    case OS: return "OS";
    case CS: return "CS";
    case DOT: return "DOT";
    case EQ: return "EQ";
    case ADD: return "ADD";
    case SUB: return "SUB";
    case MUL: return "MUL";
    case GT: return "GT";
    case LT: return "OS";
    case ASSIGN: return "ASSIGN";
    case INT: return "INT";
    case STR: return "STR";
    case BOOL: return "BOOL";
    case PRINT: return "PRINT";
    case BIND: return "BIND";
    case CONSTRICT: return "CONSTRICT";
    case FN: return "FN";
    case RETURN: return "RETURN";
    case IDENTIFIER: return "IDENTIFIER"; 
    case VAR_DECL: return "VAR_DECL"; 
    case LITERAL: return "LITERAL"; 
    case PROGRAM: return "PROGRAM"; 
  }
  return "ERROR";
}

typedef struct {
  /* Attribute value usage:
   * literals are stored as they are.
   * identifiers stored as the index into the sym table */
  char *value; // attribute value
  token_name_T type;
} token_T;

/*  ============================= GRAMMAR ====================================
 * This section describes the syntax of boa constrictor.
 * Keywords, Separators and Operators are defined here
 * =======================================================================*/

/* KEYWORDS
 * Reserved keywords for the language
 * */
token_T keywords[] = {
  {"int", INT},
  {"string", STR},
  {"bool", BOOL },
  {"print", PRINT},
  {"bind", BIND},
  {"constrict", CONSTRICT},
  {"fn", FN},
  {"return", RETURN},
  {"program", PROGRAM},
};

size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

token_T separators[] = {
  {";", SEMI},
  {"{", OCB},
  {"}", CCB},
  {"(", OP},
  {")", CP},
};

size_t separator_count = sizeof(separators) / sizeof(separators[0]);


// TODO support operators of len 2
token_T operators[] = {
  {"=", ASSIGN},
  {"==", EQ},
  {"+", ADD},
  {"-", SUB},
  {"*", MUL},
  {"<", GT},
  {">", LT},
};

size_t operator_count = sizeof(operators) / sizeof(operators[0]);

/* LITERALS
 * Rules for reading literals:
 *   Numerics: must all be numbers 
 *   Strings: must be alphanum with len >= 1 and enclosed in " "
 *   Char: must be a single alphanum enclosed in ' ' 
 * */

/*  ============================= SYMBOL TABLE ===============================
 * Symbol table that holds all of the different types of tokens
 * init_symtable() will populate the symbol table with the predefined tokens
 * =======================================================================*/
typedef struct {
  size_t size;
  size_t capacity;
  int last_pos; // last used pos in symtable
  token_T *table;
} symtable_T;

void print_symbol_table(symtable_T *t) {
  printf("\n%s\n", "===== SYMBOL TABLE =====");
  for (size_t i = 0; i <= t->size; i++) {
    printf("Token: %s\nToken Type: %s\n\n", t->table[i].value,
        get_token_name(t->table[i].type));
  }
  printf("%s\n", "========================");
}

/* returns the index of the lexeme in the symtable. 
 * returns 0 if it does not exist */
int lookup(symtable_T *t, char lexeme[]) {
  int i;
  for (i = t->last_pos; i > 0; i--) {
    if (strcmp(t->table[i].value, lexeme) == 0) return i;
  }
  return 0;
}

/* creates a token and adds it to the symtable
 * returns the position of entry of the lexeme */
int insert(symtable_T *t, char lexeme[], token_name_T type) {
  int len;
  len = strlen(lexeme);
  //FIXME need to allocate more memory
  if ((unsigned long)(t->last_pos + 1) >= t->capacity) {
    perror("error: symbol table full");
    exit(1);
  }

  t->last_pos += 1;
  t->size += 1;
  t->table[t->last_pos].value = malloc(sizeof(char) * len);
  t->table[t->last_pos].type = type;

  strncpy(t->table[t->last_pos].value, lexeme, len);

  return t->last_pos;
}

/* init and load with keywords */
symtable_T *init_symtable(size_t initial_size) {
  symtable_T *symtable = malloc(sizeof(symtable_T));
  symtable->table = malloc(sizeof(token_T) * initial_size);
  symtable->last_pos = 0;
  symtable->size = 0;
  symtable->capacity= initial_size;

  //load keywords
  for (size_t i = 0; i < keyword_count; i++) {
    insert(symtable, keywords[i].value, keywords[i].type);
  }

  //load operators
  for (size_t i = 0; i < operator_count; i++) {
    insert(symtable, operators[i].value, operators[i].type);
  }

  //load separators 
  for (size_t i = 0; i < separator_count; i++) {
    insert(symtable, separators[i].value, separators[i].type);
  }

  return symtable;
}

/* ================================ LEXER =================================
 * Lexer is responsible for reading characters from the buffer populating
 * the symbol table with tokens
 * 
 * Passes token and its attributes to the parser
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

bool is_separator(char current) {
  for (size_t i = 0; i < separator_count; i++) {
    if (current == *separators[i].value) return true;
  }
  return false;
}

bool is_keyword(char current) {
  for (size_t i = 0; i < keyword_count; i++) {
    if (current == *keywords[i].value) return true;
  }
  return false;
}

bool is_operator(char current) {
  for (size_t i = 0; i < operator_count; i++) {
    if (current == *operators[i].value) return true;
  }
  return false;
}
/* Peeks at the next character in the file buffer
 * returns -1 if an attempt is made to peek past the end of the buffer */
char peek(lexer_T *lexer, int offset) {
  if ((lexer->index + offset) >= lexer->src_size) return -1;
  return lexer->src[lexer->index + offset];
}

/* Peeks at next token in file buffer 
 * returns token type on success, error on failure */
token_name_T peek_next_token(lexer_T *lexer) {
  int p = 0;
  int b = 0;
  char buf[LEX_BUFF_CAPACITY];
  token_name_T name;
  char curr;

  /* Handle whitespace, tabs and newlines TODO hope this is ok*/
  if (lexer->current == ' ' || lexer->current == '\t') {
    lexer_skip_whitespace(lexer);
  }

  if (lexer->current == '\n') {
    lexer_skip_newline(lexer);
  }

  /* peek keyword identifer */
  if (isalpha(lexer->current)) {
    buf[b] = lexer->current;
    curr = lexer->current;
    while (isalnum(curr)) {
      b += 1;
      curr = peek(lexer, b);
      buf[b] = curr;
      if (b >= LEX_BUFF_CAPACITY) {
        perror("error: lexer buffer capacity exceeded in token peek");
        exit(1);
      }
    }
    buf[b] = '\0';

    p = lookup(lexer->symtable, buf);

    // TODO unknown in symbol table, will be an identifier ?!
    if (p == 0) return IDENTIFIER;

    name = lexer->symtable->table[p].type;
    return name;
  }

  /* peek literal string */
  else if (lexer->current == '"') {
    buf[b] = lexer->current;
    curr = lexer->current;
    while (curr != -1) {
      b += 1;
      curr = peek(lexer,b);
      buf[b] = curr;
      if (curr == '"') {
        buf[b+1] = '\0';
        return LITERAL;
      }
    }
  }

  /* peek numeric literal */
  else if (isdigit(lexer->current)) {
    b = 0;
    printf("IN NUMERIC PEEK %c", lexer->current);
    buf[b] = lexer->current;
    curr = lexer->current;
    while (curr != -1) {
      b += 1;
      curr = peek(lexer,b);
      if (!isdigit(curr)) {
        buf[b+1] = '\0';
        return LITERAL;
      }
      buf[b] = curr;
    }
  }

  /* Handle Operators */
  else if (is_operator(lexer->current)) {
    buf[b] = lexer->current;
    buf[b+1] = '\0';
    p = lookup(lexer->symtable, buf);
    name = lexer->symtable->table[p].type;
    return name;
  }

  /* Handle Separators */
  else if (is_separator(lexer->current)) {
    printf("IN peek_token(), checking for a separator\n");
    buf[b] = lexer->current;
    buf[b+1] = '\0';
    p = lookup(lexer->symtable, buf);
    name = lexer->symtable->table[p].type;
    return name;
  }

  /* Handle EOF */
  else if(lexer->current == '\0') return DONE;
  
  printf("IN peek_next_token() returning ERROR\n");
  printf("current val: %c \n", lexer->current);
  return ERROR;
}


/* Main entry point of Lexer
 * This function operates on the instance of lexer, advnaces through each 
 * and returns the index of its location in the symbol table 
 *
 * Designed to be called by the parser */
int lexer_analyze(lexer_T *lexer) {
  char lex_buff[LEX_BUFF_CAPACITY];
  int p, b = 0;

  /* Handle whitespace and tabs */
  if (lexer->current == ' ' || lexer->current == '\t') {
    lexer_skip_whitespace(lexer);
  }

  /* Handle newlines */
  else if (lexer->current == '\n') {
    lexer_skip_newline(lexer);
  }

  /* Handle identifiers */
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

  /* Handle Literals */
  // String 
  else if (lexer->current == '"') {
    char res = ' ';
    lex_buff[b] = lexer->current;
    while (res != -1) {
      b += 1;
      res = peek(lexer, b);
      lex_buff[b] = res;
      if (res == '"') {
        lex_buff[b+1] = '\0';
        p = insert(lexer->symtable, lex_buff, LITERAL);
        for (int i = 0; i <= b; i++) lexer_advance(lexer);
        return p;
      }
    }
    if (res == -1) {
      perror("error: missing a closing double quote");
      exit(1);
    }
  }

  // Integer
  else if (isdigit(lexer->current)) {
    lex_buff[b] = lexer->current;
    char curr = lexer->current;
    while (curr != -1) {
      b += 1;
      curr = peek(lexer,b);
      if (!isdigit(curr)) {
        lex_buff[b] = '\0';
        p = insert(lexer->symtable, lex_buff, LITERAL);
        for (int i = 0; i < b; i++) lexer_advance(lexer);
        return p;
      }

      lex_buff[b] = curr;
    }
  }

  /* Handle Operators */
  else if (is_operator(lexer->current)) {
    lex_buff[b] = lexer->current;
    lex_buff[b+1] = '\0';
    p = lookup(lexer->symtable, lex_buff);
    lexer_advance(lexer);
    return p;
  }

  /* Handle Separators */
  else if (is_separator(lexer->current)) {
    lex_buff[b] = lexer->current;
    lex_buff[b+1] = '\0';
    p = lookup(lexer->symtable, lex_buff);
    lexer_advance(lexer);
    return p;
  }

  /* Handle EOF */
  else if(lexer->current == '\0') return DONE;

  /* Handle Error */
  return ERROR;
}

bool lexer_teardown(lexer_T *lexer) {
  free(lexer);
  lexer = NULL;

  return true;
}

/* ================================ PARSER =================================
 * Parser receives tokens produced by the lexer and consumes them
 *
 * Responsible for grouping th etokens into grammatical phrases.
 * which are represented as a parse tree / syntax tree
 * =======================================================================*/

typedef struct node_T node_T;

struct node_T {
  node_T *children;
  int num_children;
  char *value;
  token_name_T type;
};

node_T *new_node(token_T token) {
  node_T *n = malloc(sizeof(node_T));
  n->children = malloc(MAX_CHILDREN * sizeof(node_T));
  n->num_children = 0;
  n->value = token.value;
  n->type = token.type;
  return n;
}

node_T *new_var_decl_node(token_T token) {
  node_T *n = malloc(sizeof(node_T));
  n->children = malloc(MAX_CHILDREN * sizeof(node_T));
  n->num_children = 0;
  n->value = token.value;
  n->type = VAR_DECL;
  return n;
}

typedef struct {
  lexer_T *lexer;
  node_T *ast;
} parser_T;

node_T *parse(lexer_T *lexer); //forward declaration

// print AST level to level
void print_ast(node_T *root) {
  if (root == NULL) return;

  printf("%s\n", root->value);

  for (int i = 0; i < root->num_children; i++) {
    print_ast(&root->children[i]);
  }

}
// VarDecl       <Type> <Identifier> ASSIGN <String Literal> SEMI
// TODO error checking
node_T *parse_var_decl(lexer_T *lexer) {
  int p; 
  p = lexer_analyze(lexer); // consume type
  node_T *n = new_var_decl_node(lexer->symtable->table[p]); // assign with type
  n->children[0] = *parse(lexer); // parse Identifier
  n->num_children++;
  n->children[1] = *parse(lexer); //parse assign
  n->num_children++;
  n->children[2] = *parse(lexer); // parse Literal TODO should be an expression
  n->num_children++;
  n->children[3] = *parse(lexer); // parse semi 
  n->num_children++;

  return n;
}

node_T *parse_identifier(lexer_T *lexer) {
  int p = lexer_analyze(lexer);
  return new_node(lexer->symtable->table[p]);
}

node_T *parse_literal(lexer_T *lexer) {
  int p = lexer_analyze(lexer);
  return new_node(lexer->symtable->table[p]);
}

node_T *parse_assign_op(lexer_T *lexer) {
  int p = lexer_analyze(lexer);
  return new_node(lexer->symtable->table[p]);
}

node_T *parse_semi(lexer_T *lexer) {
  int p = lexer_analyze(lexer);
  return new_node(lexer->symtable->table[p]);
}

void append_children(node_T *root, node_T *new_node) {
  root->children[root->num_children] = *new_node;
  root->num_children++;
}

node_T *parse(lexer_T *lexer) {
  token_name_T curr = peek_next_token(lexer);

  // Build AST
  switch (curr) {
    case (STR): 
      return parse_var_decl(lexer); 
    case (INT): 
      return parse_var_decl(lexer); 
    case (BOOL): 
      return parse_var_decl(lexer); 

    case (IDENTIFIER): 
      return parse_identifier(lexer);
    case (LITERAL): 
      return parse_literal(lexer);
    case (ASSIGN): 
      return parse_assign_op(lexer);
    case (SEMI): 
      return parse_semi(lexer);
    default:
      printf("Error with token: %s", get_token_name(curr));
      perror("error: unable to construct AST");
      exit(1);
  }
}

parser_T * run_parser(lexer_T *lexer) {
  parser_T *parser = malloc(sizeof(parser_T));
  parser->lexer = lexer;
  int p = lookup(lexer->symtable, "program"); 

  node_T *root = new_node(lexer->symtable->table[p]);
  
  while (peek_next_token(lexer) != DONE) {
    append_children(root, parse(lexer));
  }

  parser->ast = root;

  return parser;
}

bool parser_teardown(parser_T *parser) {
  free(parser);
  parser = NULL;

  return true;
}
/* ============================ CODE GENERATOR ==============================
 * Code Generator Stub
 * =======================================================================*/

// pass

/* ================================ DRIVER =================================
 * Where all of the magic happens!
 * =======================================================================*/
size_t get_file_size(FILE *f) {
  size_t file_size;
  if (fseek(f, 0, SEEK_END) != 0 ) exit(EXIT_FAILURE); 

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

  /* File buffer setup */
  size_t file_size = get_file_size(f);
  char *buffer = malloc(file_size * sizeof(char));

  read_file(f, buffer, file_size);

  printf("%s\n", buffer); 

  /* Lexer entry point */
  printf("\n%s\n", "====== Lexer ======");
  lexer_T *lexer = init_lexer(buffer, file_size);

  /* Parser entry point */
  parser_T *parser = run_parser(lexer);

  // parser should have an AST setup now
  printf("\n%s\n", "AST");

  print_ast(parser->ast);
  /* Print contents of symbol table */
  // print_symbol_table(lexer->symtable);

  free(buffer);

  return 0;
}
