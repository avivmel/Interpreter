//
//  ASTNode.h
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
    // ~ASTNode();
    
    void printBT(const std::string prefix, const ASTNode* node, bool isLeft) {
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

    friend std::ostream& operator<<(std::ostream &os, ASTNode* node) {

        node->printBT("", node, false);

        // os << "Token(" << EnumToString(t.tokenType) << ", " << t.value << ")";
        os << "";

        return os;
    }
};



class StatementsVector : public ASTNode {
   public:
   std::vector<ASTNode*> vector;
    
    StatementsVector() {
        
    }

   StatementsVector(std::vector<ASTNode*> Vector) {
       vector = Vector;
   }
};

class Function : public ASTNode {
    public:
    StatementsVector* vector;
    std::string ID;

   Function(StatementsVector* Vector, Token id) {
       vector = Vector;
       ID = id.value;
   }
};

class FunctionVector : public ASTNode {
   public:
   std::vector<Function*> vector;

   FunctionVector(std::vector<Function*> Vector) {
       vector = Vector;
   }
};

// class BinaryOpNode : public ASTNode {
   

// };


#endif /* ASTNode_h */
