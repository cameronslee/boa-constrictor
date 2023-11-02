#ifndef TOKEN_H
#define TOKEN_H 

#define DONE 69420 

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

const char * get_token_name(token_name_T type);

#endif


