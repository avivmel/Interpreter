# Interpreter

This is a simple interpreted programming language implamented in C++.

## The Language
The REPL currently only supports assignment statements and mathematical expressions.

```
>> x = 7 + 3    /* 10 */
>> y = x / 2    /* 5  */
>> z = x + y    /* 15 */
```
## Syntax rules in BNF
```
factor : PLUS factor
     | MINUS factor
     | INTEGER
     | LPAREN expr RPAREN
     | variable

term: factor ((MUL | DIV) factor)*

expr: term ((PLUS | MINUS) term)*

assignment_statement : variable ASSIGN expr

statement_list : statement
                | statement SEMI statement_list

```
## Instructions
Download the repo and run ```main.cpp``` to access the REPL.

