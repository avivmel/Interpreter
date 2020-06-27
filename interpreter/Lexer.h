//
//  Lexer.h
//  interpreter
//
//  Created by Aviv Melamud on 6/24/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#ifndef Lexer_h
#define Lexer_h

#include "TOKENTYPE.h"

class Lexer {
public:
    
    std::string text;
    int pos = 0;
    char current_char;
    
    Lexer() {
        pos = 0;
    }
    
    Lexer(std::string Text) {
        text = Text;
        current_char = text[pos];
    }
    
    int error() {
        throw std::runtime_error("error");
    }
    
    void advance() {
        /*
        Move the 'pos' pointer +1 and set the 'current_char' var.
        */
        
        pos += 1;
        if (pos > text.length() - 1) {
            current_char = '\0';
        } else {
            current_char = text[pos];
        }
    }
    
    void skip_whitespace() {
        while (current_char != '\0' && current_char == ' ') {
            advance();
        }
    }
    
    std::string integer() {
        /*
        @Return a (multidigit) integer selected from the text string.
        */
        std::string muliCharInt = "";
        while (current_char != '\0' && isdigit(current_char)) {
            // std::cout << "digit " << current_char << "\n";
            muliCharInt += current_char;
            advance();
        }
        return muliCharInt;
    }
    
    Token getNextToken() {
        
        /*Lexical analyzer (also known as scanner or tokenizer)

        Breaks a "sentence" apart into tokens of given.
        @return a token object.
        */
        
        while (current_char != '\0') {
            
            //std::cout << "current_char: " << current_char << "\n";
            

            
            if (pos > text.length() - 1) {
                return Token(TOKENTYPE::EOL, "");
            }
            
            
            if (current_char == ' ') {
                skip_whitespace();
                continue;
            }
            
            if (isdigit(current_char)) {
                Token token = Token(TOKENTYPE::INTEGER, integer());
                std::cout << token << "\n";
                return token;
            }
            if (current_char == '+') {
                Token token = Token(TOKENTYPE::PLUS, std::string(1, current_char));
                advance();
                std::cout << token << "\n";
                return token;
            }
            if (current_char == '-') {
                Token token = Token(TOKENTYPE::MINUS, std::string(1, current_char));
                advance();
                std::cout << token << "\n";
                return token;
            }
            if (current_char == '*') {
                Token token = Token(TOKENTYPE::MUL, std::string(1, current_char));
                advance();
                std::cout << token << "\n";
                return token;
            }
            if (current_char == '/') {
                Token token = Token(TOKENTYPE::DIV, std::string(1, current_char));
                advance();
                std::cout << token << "\n";
                return token;
            }
            if (current_char == '(') {
                Token token = Token(TOKENTYPE::LPAREN, std::string(1, current_char));
                advance();
                std::cout << token << "\n";
                return token;
            }
            if (current_char == ')') {
                Token token = Token(TOKENTYPE::RPAREN, std::string(1, current_char));
                advance();
                std::cout << token << "\n";
                return token;
            }

            
            std::cout << "char '" << current_char << "' not found\n";
            error();
        }
        return Token();

    }
    
    
    
};

#endif /* Lexer_h */
