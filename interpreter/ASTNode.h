//
//  Lexer.h
//  interpreter
//
//  Created by Aviv Melamud on 6/26/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#include <iostream>
#include <string>
#include <memory>

#ifndef ASTNode_h
#define ASTNode_h

#include "TOKENTYPE.h"
#include "Token.hpp"


class ASTNode {
    public:
    ASTNode* left;
    ASTNode* right;
    Token token;
    ASTNode() {
        left = NULL;
        right = NULL;
    }
    ASTNode(ASTNode* Left, ASTNode* Right, Token T) {
        left = Left;
        right = Right;
        token = T;
    }
    ~ASTNode();
};

// class NumberNode : public ASTNode {
   
// };

// class BinaryOpNode : public ASTNode {
   

// };


#endif /* ASTNode_h */
