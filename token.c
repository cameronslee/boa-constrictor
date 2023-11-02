#include "token.h" 

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
