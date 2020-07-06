# Interpreter

This is a simple interpreted programming language implamented in C++.

## The Language
The REPL currently only supports assignment statements and mathematical expressions.

```
FUNC defineY
     y = 5;
END

FUNC main
     x = 7 + 3;    /* 10 */
     defineY();    /* Functions currently operate on the global scope  */
     z = x + y;    /* 15 */


END
```
## Syntax rules in BNF
```
factor : PLUS factor
     | MINUS factor
     | INTEGER
     | LPAREN expr RPAREN
     | Variable
     | Func call

term: factor ((MUL | DIV) factor)*

expr: term ((PLUS | MINUS) term)*

assignment_statement : variable ASSIGN expr

statement_vector : statement
                | statement SEMI statement_list

function : FUNC ID statement_list END

function_vector : function*

```


## Instructions
Download the repo and run ```main.cpp``` to access the REPL.

