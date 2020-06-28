//
//  main.cpp
//  interpreter
//
//  Created by Aviv Melamud on 6/23/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#include <iostream>
#include <string>


#include "TOKENTYPE.h"
#include "Token.hpp"
#include "Lexer.h"
#include "ASTNode.h"



class Interpreter {
public:
    Lexer lexer;
    Token currentToken;

    
    Interpreter(std::string Text) {
        lexer = Lexer(Text);
        std::cout << "text: " << lexer.text << "\n";
        currentToken = lexer.getNextToken();
        
        
    }
    
    int error(std::string errorStr) {
        throw std::runtime_error(errorStr);
    }
    
    
    
    void checkAndGetNxt(TOKENTYPE expectedTokenType) {
        if (currentToken.tokenType == expectedTokenType) {
            currentToken = lexer.getNextToken();
            //std::cout << currentToken << "\n";
        } else {
            std::cout << "type " << EnumToString(currentToken.tokenType) << " ex " << EnumToString(expectedTokenType) << "\n";
            error("checkAndGetNxt incorrect type");
            
        }
    }
    
    ASTNode* factor() {
        Token token = currentToken;
        // std::cout << "fpre " << token << "\n";

        if (token.tokenType == TOKENTYPE::INTEGER) {
            checkAndGetNxt(TOKENTYPE::INTEGER);
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
            ASTNode* parenContents = evalExpr();
            checkAndGetNxt(TOKENTYPE::RPAREN);
            return parenContents;
        }
        
        error("Token(" + EnumToString(currentToken.tokenType) + ", " + currentToken.value + ") passed to factor()");
        return 0;
    }
    
    
     ASTNode* term() {
        //Token token = lexer.getNextToken();
        ASTNode* initalTermVal = factor();
        //std::cout << "tpre " << termVal << "\n";

        
        while (currentToken.tokenType == TOKENTYPE::MUL || currentToken.tokenType == TOKENTYPE::DIV) {
            Token token = currentToken;
            if (currentToken.tokenType == TOKENTYPE::MUL) {
                checkAndGetNxt(TOKENTYPE::MUL);
                //termVal *= factor();
                
            }
            if (currentToken.tokenType == TOKENTYPE::DIV) {
                checkAndGetNxt(TOKENTYPE::DIV);
                //termVal /= factor();
            }
            ASTNode* termVal = new ASTNode(initalTermVal, factor(), token);
            return termVal;

        }
        return initalTermVal;
        


    }
        
    ASTNode* evalExpr() {
    
        
        /*
         Parser / Interpreter
         
         Detect expressions
        */
        
    
        ASTNode* termVal = term();
        
        while (currentToken.tokenType == TOKENTYPE::PLUS || currentToken.tokenType == TOKENTYPE::MINUS) {
            Token token = currentToken;
            if (currentToken.tokenType == TOKENTYPE::PLUS) {
                checkAndGetNxt(TOKENTYPE::PLUS);
                //exprVal += term();
                
            }
            if (currentToken.tokenType == TOKENTYPE::MINUS) {
                checkAndGetNxt(TOKENTYPE::MINUS);
                //exprVal -= term();
            }
            termVal = new ASTNode(termVal, term(), token);
        }

        
        
        return termVal;
    }
    
    int visit(ASTNode* node) {
        if (node->token.tokenType == TOKENTYPE::PLUS) {
            return visit(node->left) + visit(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::MINUS) {
            return visit(node->left) - visit(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::MUL) {
            return visit(node->left) * visit(node->right);
        }
        if (node->token.tokenType == TOKENTYPE::INTEGER) {
            return stoi(node->token.value);
        }
        return 0;
    }

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
       ASTNode* node2 = interpreter.evalExpr();
       std::cout << "value = " << interpreter.visit(node2) << "\n\n\n";
       interpreter.printBT("", node2, false);
       interpreter.deallocTree(node2);
       std::cout << "\n\n";
       //interpreter.error();
   }

    // ASTNode *n1 = new ASTNode(NULL, NULL, Token(TOKENTYPE::INTEGER, "5"));

    // ASTNode *node2 = new ASTNode(n1, n1, Token(TOKENTYPE::EMPTY, ""));

    //cout node2->left->token
    

    



    return 0;

}
