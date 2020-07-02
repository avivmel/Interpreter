//
//  main.cpp
//  interpreter
//
//  Created by Aviv Melamud on 6/23/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
#include <vector>


#include "TOKENTYPE.h"
#include "Token.hpp"
#include "Lexer.h"
#include "ASTNode.h"



class Interpreter {
public:
    Lexer lexer;
    Token currentToken;
    std::map<std::string, int> GLOBAL_SYMBOL_TABLE;


    
    Interpreter(std::string Text) {
        lexer = Lexer(Text);
        std::cout << "text: " << lexer.text << "\n";
        currentToken = lexer.getNextToken();
        
        
    }
    
    int error(std::string errorStr) {
        throw std::runtime_error(errorStr);
    }
    
    
    
    void checkAndGetNxt(TOKENTYPE expectedTokenType) {
        /*
        Checks if the currentToken type attribute matches the type that the parser expects to recieve. 
        If it dosen't, the 'Unexpected token' error is invoked.
        */
        if (currentToken.tokenType == expectedTokenType) {
            currentToken = lexer.getNextToken();
            //std::cout << currentToken << "\n";
        } else {
            std::cout << "Unexpected token: type " << EnumToString(currentToken.tokenType) << " expected, got " << EnumToString(expectedTokenType) << "\n";
            error("checkAndGetNxt incorrect type");
            
        }
    }
    
    /*
     factor : PLUS factor
     | MINUS factor
     | INTEGER
     | LPAREN expr RPAREN
     | variable
     
     Factor is the smallest part of an expression
     */
    ASTNode* factor() {

         
        Token token = currentToken;
        // std::cout << "fpre " << token << "\n";

        if (token.tokenType == TOKENTYPE::INTEGER) {
            checkAndGetNxt(TOKENTYPE::INTEGER);
            ASTNode* node = new ASTNode(NULL, NULL, token);
            return node;
            
        } if (token.tokenType == TOKENTYPE::ID) {
            checkAndGetNxt(TOKENTYPE::ID);
            ASTNode* node = new ASTNode(NULL, NULL, token);
            return node;
            
        } else if (token.tokenType == TOKENTYPE::MINUS) {
            checkAndGetNxt(TOKENTYPE::MINUS);
            ASTNode* node = new ASTNode(factor(), new ASTNode(NULL, NULL, Token(TOKENTYPE::INTEGER, "-1")), Token(TOKENTYPE::MUL, "*"));
            return node;
        } else if (token.tokenType == TOKENTYPE::PLUS) {
            checkAndGetNxt(TOKENTYPE::PLUS);
            ASTNode* node = new ASTNode(factor(), new ASTNode(NULL, NULL, Token(TOKENTYPE::INTEGER, "1")), Token(TOKENTYPE::MUL, "*"));
            return node;
        
        } else if (token.tokenType == TOKENTYPE::LPAREN) {
            checkAndGetNxt(TOKENTYPE::LPAREN);
            // If an open parenthesies is found, recursivly call the evaluate expression function to find its contents
            ASTNode* parenContents = evalExpr();
            checkAndGetNxt(TOKENTYPE::RPAREN);
            return parenContents;
        }
        
        error("Token(" + EnumToString(currentToken.tokenType) + ", " + currentToken.value + ") passed to factor()");
        return 0;
    }
    
    
    /*
     term: factor ((MUL | DIV) factor)*
     */
     ASTNode* term() {
        ASTNode* initalTermVal = factor(); // this is the factor that will multiplied or divided

        
        while (currentToken.tokenType == TOKENTYPE::MUL || currentToken.tokenType == TOKENTYPE::DIV) {
            Token token = currentToken;
            if (currentToken.tokenType == TOKENTYPE::MUL) {
                checkAndGetNxt(TOKENTYPE::MUL);
                
            }
            if (currentToken.tokenType == TOKENTYPE::DIV) {
                checkAndGetNxt(TOKENTYPE::DIV);
            }
            ASTNode* termVal = new ASTNode(initalTermVal, factor(), token);
            return termVal;

        }
        return initalTermVal; // if no MUL or DIV is found, return the factor
        


    }
    
    /*
    Expr: term ((PLUS | MINUS) term)*
    EvalExpr creates an tree from a mathematical expression. PLUS and MINUS are last in the order of operations,
    so they must be evaluated first in the tree.
    */
    ASTNode* evalExpr() {
    
        ASTNode* termVal = term();
        
        while (currentToken.tokenType == TOKENTYPE::PLUS || currentToken.tokenType == TOKENTYPE::MINUS) {
            Token token = currentToken;
            if (currentToken.tokenType == TOKENTYPE::PLUS) {
                checkAndGetNxt(TOKENTYPE::PLUS);
                
            }
            if (currentToken.tokenType == TOKENTYPE::MINUS) {
                checkAndGetNxt(TOKENTYPE::MINUS);
            }
            termVal = new ASTNode(termVal, term(), token);
        }

        
        
        return termVal;
    }

    /*
     assignment_statement : variable ASSIGN expr
     */
    ASTNode* assignment_statement() {

        if (currentToken.tokenType == TOKENTYPE::ID) {
            ASTNode* variable = var();
            return variable;
        }
        ASTNode* expr = evalExpr();
        std::cout << "xpr " << expr->token << "\n";
        
        return expr;

            
        
    }
    
    /*
     statement_list : statement
                    | statement SEMI statement_list
     
     @returns a vector of assignment statements (AST node pointers)
     */
    std::vector<ASTNode*> statements_list() {
        std::vector<ASTNode*> list;
        list.push_back(assignment_statement());

        while (currentToken.tokenType == TOKENTYPE::SEMI) {
            checkAndGetNxt(TOKENTYPE::SEMI);
//            if (list.size() == 2) {
//                error("2");
//            }
            if (currentToken.value != "") {
                ASTNode* statement = assignment_statement();
                
                if (statement != NULL) {
                    list.push_back(statement);
                }
            }
            
        }

        return list;
        
    }

    /*
    variable : ID ASSIGN expr
    */
    ASTNode* var() {
        Token token = currentToken;

        if (token.tokenType == TOKENTYPE::ID) {
            Token id = token;
            checkAndGetNxt(TOKENTYPE::ID);
            
            Token assign = currentToken;
            checkAndGetNxt(TOKENTYPE::ASSIGN);
            ASTNode* expr = evalExpr();
            
            ASTNode* var_id = new ASTNode(NULL, NULL, id);

            return new ASTNode(var_id, expr, assign);
        }
        error("var not found, instead " + EnumToString(token.tokenType) + " was passed");
        return new ASTNode(NULL, NULL, token);;
    }
    
    /*
     @params:
     node - an assignment statement tree
     visit_expr recursivly traverses an abstract syntax tree and preforms all operations,
     and @returns its int value
     */
    int visit_expr(ASTNode* node) {
        if (node->token.tokenType == TOKENTYPE::PLUS) {
            return visit_expr(node->left) + visit_expr(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::MINUS) {
            return visit_expr(node->left) - visit_expr(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::MUL) {
            return visit_expr(node->left) * visit_expr(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::DIV) {
            return visit_expr(node->left) / visit_expr(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::INTEGER) {
            return stoi(node->token.value);
        }
        if (node->token.tokenType == TOKENTYPE::ASSIGN) {
            int expr_val = visit_expr(node->right);
            GLOBAL_SYMBOL_TABLE[node->left->token.value] = expr_val; // add variable to symbol table
            return expr_val;
        }
        if (node->token.tokenType == TOKENTYPE::ID) {
            if (GLOBAL_SYMBOL_TABLE.find(node->token.value) != GLOBAL_SYMBOL_TABLE.end()) { // check if variable has been initiated
                return GLOBAL_SYMBOL_TABLE[node->token.value]; // get its value from the symbol table
            }
            error("Variable used not yet defined");
            return 0;
        }
        return 0;
    }

    /*
     Recursivly traverses an abstract syntax tree and frees the
     HEAP memory it takes up
     */
    void deallocTree(ASTNode* node) {
        if (node->left != NULL) {
            deallocTree(node->left);
        }
        if (node->right != NULL) {
            deallocTree(node->right);
        }

        free(node);
    }
    
    void printBT(const std::string& prefix, const ASTNode* node, bool isLeft)
    {
        if( node != nullptr )
        {
            std::cout << prefix;
            std::cout << (isLeft ? "├── " : "└── " );

            // print the value of the node
            std::cout << node->token.value << std::endl;

            // enter the next tree level - left and right branch
            printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
            printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }
    
    
};

    


int main(int argc, const char * argv[]) {



   while (true) {
       std::cout << ">> ";
       
       std::string text;
       getline(std::cin, text);

       Interpreter interpreter(text);
       std::vector<ASTNode*> tree = interpreter.statements_list();
       // std::cout << typeid(tree).name() << "\n";

       for (int i=0; i < tree.size(); i++) {
           std::cout << tree[i]->left->token.value << " value = " << interpreter.visit_expr(tree[i]) << "\n\n";
           interpreter.printBT("", tree[i], false);
           interpreter.deallocTree(tree[i]);
           std::cout << "\n";
       }
       //interpreter.error(" ");
       //interpreter.deallocTree(tree);
       std::cout << "\n\n";
       //interpreter.error();
   }

    

    



    return 0;

}
