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

    ASTNode* assignment_statement() {

        if (currentToken.tokenType == TOKENTYPE::ID) {
            ASTNode* variable = var();
            return variable;
        }
//        else {
//            ASTNode* expr = evalExpr();
//            if (expr->token != NULL) {
//                return expr
//            }
//        }
        ASTNode* expr = evalExpr();
        std::cout << "xpr " << expr->token << "\n";
        
        return expr;

            
        
    }

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
            GLOBAL_SYMBOL_TABLE[node->left->token.value] = expr_val;
            return expr_val;
        }
        if (node->token.tokenType == TOKENTYPE::ID) {
            if (GLOBAL_SYMBOL_TABLE.find(node->token.value) != GLOBAL_SYMBOL_TABLE.end()) {
                return GLOBAL_SYMBOL_TABLE[node->token.value];
            }
            error("Variable used not yet defined");
            return 0;
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
       std::vector<ASTNode*> tree = interpreter.statements_list();
       // std::cout << typeid(tree).name() << "\n";

       //std::cout << "value = " << interpreter.visit_expr(tree[0]->right) << "\n\n\n";
       for (int i=0; i < tree.size(); i++) {
           std::cout << tree[i]->left->token.value << " value = " << interpreter.visit_expr(tree[i]) << "\n\n";
           interpreter.printBT("", tree[i], false);
           std::cout << "\n";
       }
       //interpreter.error(" ");
       interpreter.deallocTree(tree);
       std::cout << "\n\n";
       //interpreter.error();
   }

    // ASTNode *n1 = new ASTNode(NULL, NULL, Token(TOKENTYPE::INTEGER, "5"));

    // ASTNode *node2 = new ASTNode(n1, n1, Token(TOKENTYPE::EMPTY, ""));

    //cout node2->left->token
    

    



    return 0;

}
