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

#include <iostream>
#include "TOKENTYPE.h"
#include "Token.hpp"

/* Node object used to construct the abstract syntax tree, which is organized to
represent correct order of operations and scoping.

For example: 5 + 2 * 4 is represented as:
└── +
    ├── 5
    └── *
        ├── 2
        └── 4
*/

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
