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



class Interpreter {
public:
    Lexer lexer;
    Token currentToken;

    
    Interpreter(std::string Text) {
        lexer = Lexer(Text);
        std::cout << "text: " << lexer.text << "\n";
        currentToken = lexer.getNextToken();
        
        
    }
    
    int error() {
        throw std::runtime_error("error");
    }
    
    
    
    void checkAndGetNxt(TOKENTYPE expectedTokenType) {
        if (currentToken.tokenType == expectedTokenType) {
            currentToken = lexer.getNextToken();
            //std::cout << currentToken << "\n";
        } else {
            std::cout << "type " << EnumToString(currentToken.tokenType) << " ex " << EnumToString(expectedTokenType) << "\n";
            error();
            
        }
    }
    
    int factor() {
        Token token = currentToken;
        // std::cout << "fpre " << token << "\n";

        if (token.tokenType == TOKENTYPE::INTEGER) {
            checkAndGetNxt(TOKENTYPE::INTEGER);
            return stoi(token.value);
            
        } else if (token.tokenType == TOKENTYPE::LPAREN) {
            checkAndGetNxt(TOKENTYPE::LPAREN);
            int parenContents = evalExpr();
            checkAndGetNxt(TOKENTYPE::RPAREN);
            return parenContents;
        }
        
        error();
        return 0;
    }
    
    
    
    int term() {
        //Token token = lexer.getNextToken();
        int termVal = factor();
        std::cout << "tpre " << termVal << "\n";

        
        while (currentToken.tokenType == TOKENTYPE::MUL || currentToken.tokenType == TOKENTYPE::DIV) {
            if (currentToken.tokenType == TOKENTYPE::MUL) {
                checkAndGetNxt(TOKENTYPE::MUL);
                termVal *= factor();
                
            }
            if (currentToken.tokenType == TOKENTYPE::DIV) {
                checkAndGetNxt(TOKENTYPE::DIV);
                termVal /= factor();
            }
        }
        return termVal;
        


    }
        
    int evalExpr() {
    
        
        /*
         Parser / Interpreter
         
         Detect expressions
        */
        
    
        int exprVal = term();
        
        while (currentToken.tokenType == TOKENTYPE::PLUS || currentToken.tokenType == TOKENTYPE::MINUS) {
            if (currentToken.tokenType == TOKENTYPE::PLUS) {
                checkAndGetNxt(TOKENTYPE::PLUS);
                exprVal += term();
                
            }
            if (currentToken.tokenType == TOKENTYPE::MINUS) {
                checkAndGetNxt(TOKENTYPE::MINUS);
                exprVal -= term();
            }
        }

        
        
        return exprVal;
    }
    
    
};

    


int main(int argc, const char * argv[]) {



   while (true) {
       std::cout << ">> ";

       std::string text;
       getline(std::cin, text);

       Interpreter interpreter(text);
       std::cout << interpreter.evalExpr();
       std::cout << "\n";
   }


   return 0;

}
