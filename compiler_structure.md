# Structure of a Compiler
```
-------------------------
Lexical Analysis (Lexer)
Goal: Read characters from the input and group them into token objects

Lexeme: a sequence of input characters that comprises a single token

Need some way of skipping white space when reading input tokens (peek)

Need some way to read ahead (probably only one character ahead) (peek)


-------------------------
      |
      |
      v
Syntax Analysis (Parser)
      |
      |
      v
Semantic Analysis
      |
      |
      v
Intermediate Code Generator
      |
      |
      v
Code Optimizer 
      |
      |
      v
Code Generator (Generate Machine Code)

```

Notes
- parser will call the lexer multiple times until it has
  reached the end of the file and the lexer has generated
  all of the necessary tokens


# Design Specs
## Notes: One pass compiler

```
// Current stage of development
Lexer stage (To generate tokens) -> Parser (perform syntactical analysis with the tokens)
```
