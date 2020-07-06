//
//  SymbolTable.h
//  interpreter
//
//  Created by Aviv Melamud on 7/3/20.
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

int error(std::string errorStr) {
        throw std::runtime_error(errorStr);
}

class SymbolTable {
public:
    std::map<std::string, TOKENTYPE> GLOBAL_SYMBOL_TABLE;

    SymbolTable() {
        
    }
    
    /*
     Recursivly traverse an AST to determin its datatype.
     If data of different type is being combined (*, /, +, or -) an error is thrown.
     
     @params The AST of an expression
     @returns a datatype (TOKENTYPE enum)
     
     */
    TOKENTYPE getExprType(ASTNode* node) {
        
        //std::cout << "rec type " << EnumToString(node->token.tokenType) << "\n";
        
        if (node->token.tokenType == TOKENTYPE::PLUS || node->token.tokenType == TOKENTYPE::MINUS || node->token.tokenType == TOKENTYPE::MUL || node->token.tokenType == TOKENTYPE::DIV) {
            if (getExprType(node->left) == getExprType(node->right)) {
                return getExprType(node->left);
            }
        }
       
        if (node->token.tokenType == TOKENTYPE::INTEGER) {
            return TOKENTYPE::INTEGER;
        }
        
        if (node->token.tokenType == TOKENTYPE::ID) {
            return getVarType(node->token.value);
        }
        
        error("Invalid variable type");
        return TOKENTYPE::EMPTY;

    }
    
    /*
     Adds a variables from StatementsVector to GLOBAL_SYMBOL_TABLE map
     */
    void addVarsFromStatementsVector(StatementsVector* statementVectorNode) {
        for (int i=0; i < statementVectorNode->vector.size(); i++) {
            
            if (statementVectorNode->vector[i]->token.tokenType == TOKENTYPE::ASSIGN) {
                std::string varID = statementVectorNode->vector[i]->left->token.value;
                TOKENTYPE type = getExprType(statementVectorNode->vector[i]->right);
                GLOBAL_SYMBOL_TABLE[varID] = type;
            }
                
        }
    }
    
    void addFuncsFromFunctionVector(FunctionVector* functionVector) {
        for (int i=0; i < functionVector->vector.size(); i++) {
            
            Function* function = functionVector->vector[i];
            
            addVarsFromStatementsVector(function->vector);
            
            GLOBAL_SYMBOL_TABLE[function->ID] = TOKENTYPE::VOID;
            
            
        }
        
    }
    
    /*
     Queries the GLOBAL_SYMBOL_TABLE for a variable name
     */
    TOKENTYPE getVarType(std::string ID) {
        if (GLOBAL_SYMBOL_TABLE.find(ID) != GLOBAL_SYMBOL_TABLE.end()) { // check if variable has been initiated
            return GLOBAL_SYMBOL_TABLE[ID]; // get its type from the symbol table
        }
        error("Variable used has not declared");
        return TOKENTYPE::EMPTY;
    }
    
    friend std::ostream& operator<<(std::ostream &os, SymbolTable& st) {
        
        for (auto const& pair: st.GLOBAL_SYMBOL_TABLE) {
            std::cout << "{" << pair.first << ": " << EnumToString(pair.second) << "}\n";
        }

        os << "";
        return os;
    }

    

    
    
    
};
