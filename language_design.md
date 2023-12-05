# This is a workshop of terms that the language will consist of

## Coil
Circular buffer

## Coil
Circular buffer


``` 
Chat Gippity has some ideas
1. **Squeeze**: A compression function to reduce memory usage.
2. **Constrict**: A security function to limit access or permissions.
3. **Coil**: A data structure or algorithm for organizing data in a circular manner.
4. **Wrap**: A function to encapsulate or package data.
5. **Crush**: An error-handling function for handling critical failures.
6. **Strangle**: A resource management function, like closing a file or network connection.
7. **Tighten**: A performance optimization function for efficiency.
8. **Loop**: A control structure to iterate through a series of operations.
9. **Bind**: A function to associate a variable with a specific value or resource.
10. **Envelop**: A function that encapsulates data or an object.
11. **Compress**: A data compression function to reduce storage space.
12. **Encircle**: A function for checking or validating conditions.
13. **Snare**: A function for trapping or capturing errors or exceptions.
14. **Grasp**: A function to retrieve or access data.
15. **Engulf**: A function to encompass or include multiple elements.
16. **Wind**: A function for configuring or setting up a process.
17. **Constrain**: A function for limiting or controlling values.
18. **Clasp**: A function for securing or locking resources.
19. **Tangle**: A function for managing complex or interrelated data structures.
20. **Hug**: A function for grouping or nesting code blocks or elements.
```


# EBNF Spec

## ENBF TOC

The following represents a proposed ISO/IEC 14977 standard, by R. S. Scowen, page 7, tables 1 and 2.

| Usage                 | Notation         | Alternative          | Meaning         |
|-----------------------|------------------|----------------------|-----------------|
| definition            | `=`              |                      |                 |
| concatenation         | `,`              |                      |                 |
| termination           | `;`              | `.`                  |                 |
| alternation           | `!`              | `/ or !`             |                 |
| optional              | `[ ... ]`        | `(/ ... /)`          | none or once    |
| repetition            | `{ ... }`        | `(: ... :)`          | none or more    |
| grouping              | `( ... )`        |                      |                 |
| terminal string       | `" ... "`        |                      |                 |
| terminal string       | `' ... '`        |                      |                 |
| comment               | `(* ... *)`      |                      |                 |
| special sequence      | `? ... ?`        |                      |                 |
| exception             | `-`              |                      |                 |

``` 
program = block .

block = { <VarDecl> }

VarDecl = <Type> <Identifier> ASSIGN <String Literal> SEMI

Type = STR | INT | BOOL

```
