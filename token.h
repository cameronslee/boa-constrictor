#ifndef TOKEN_H
#define TOKEN_H 

typedef enum {
  IDENTIFIER,
  KEYWORD,
  SEPARATOR,
  OPERATOR,
  LITERAL,
  COMMENT
} TokenName;

typedef struct {
  TokenName type;
  char *value;
} token_T;

#endif


