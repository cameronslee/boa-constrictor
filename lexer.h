#ifndef LEXER_H
#define LEXER_H

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
} Token; 

typedef struct {
  const Token Tokens;
  unsigned NumTokens;
  unsigned index;
} Lexer;

#endif
