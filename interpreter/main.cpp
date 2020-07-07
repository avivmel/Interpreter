//
//  main.cpp
//  interpreter
//
//  Created by Aviv Melamud on 6/23/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>


#include "TOKENTYPE.h"
#include "Token.hpp"
#include "Lexer.h"
#include "ASTNode.h"
#include "Parser.h"
#include "SymbolTable.h"

class Interpreter {
public:

    std::map<std::string, void*> GLOBAL_MEMORY;
    SymbolTable symbolTable;
    
    void printGM() {
        for (auto const& pair: GLOBAL_MEMORY) {
            std::cout << "{" << pair.first << ": ";

            TOKENTYPE varType = symbolTable.getVarType(pair.first);

            if (varType == TOKENTYPE::INTEGER) {
                std::cout << static_cast<int>(reinterpret_cast<intptr_t>(GLOBAL_MEMORY[pair.first])) << "}\n"; // get its value from the symbol table and convert to int
            }
            // if (varType == TOKENTYPE::VOID) {
            //     visitStatementsVector(static_cast<Function*>(GLOBAL_MEMORY[node->token.value])->vector); // get its value from the symbol table and convert to int
            // }

            // << (pair.second) << "}\n";
        }
    }

    Interpreter(FunctionVector* functionVector, SymbolTable t) {
        symbolTable = t;
        
    }

    void visitFunctionVector(FunctionVector* functionVector) {

        for (int i=0; i < functionVector->vector.size(); i++) {

            std::string funcID = functionVector->vector[i]->ID;
            visitFunc(functionVector->vector[i]);

        }
    }

    void visitFunc(Function* function) {

        std::string funcID = function->ID;

        if (funcID == "main") {
            visitStatementsVector(function->vector);
        } else {
            GLOBAL_MEMORY[funcID] = function;
            std::cout << funcID << " added to gm\n";
        }

    }

    void visitStatementsVector(StatementsVector* statementsVector) {

        for (int i=0; i < statementsVector->vector.size(); i++) {
            visit_expr(statementsVector->vector[i]);
        }
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
            //GLOBAL_MEMORY[node->left->token.value] = &expr_val; // add variable to symbol table
            GLOBAL_MEMORY[node->left->token.value] = reinterpret_cast<void *>(static_cast<intptr_t>(expr_val));
            return expr_val;
        }
        if (node->token.tokenType == TOKENTYPE::ID) {

            if (GLOBAL_MEMORY.find(node->token.value) != GLOBAL_MEMORY.end()) { // check if variable has been initiated

                TOKENTYPE varType = symbolTable.getVarType(node->token.value);

                if (varType == TOKENTYPE::INTEGER) {
                    return static_cast<int>(reinterpret_cast<intptr_t>(GLOBAL_MEMORY[node->token.value])); // get its value from the symbol table and convert to int
                }
                if (varType == TOKENTYPE::VOID) {
                    visitStatementsVector(static_cast<Function*>(GLOBAL_MEMORY[node->token.value])->vector); // get its value from the symbol table and convert to int
                }

            }
            //error("Variable used not yet defined");
            return 0;
        }
        if (node->token.tokenType == TOKENTYPE::FUNCCALL) {
            if (GLOBAL_MEMORY.find(node->token.value) != GLOBAL_MEMORY.end()) { // check if func has been initiated
            visitStatementsVector(static_cast<Function*>(GLOBAL_MEMORY[node->token.value])->vector); // get its value from the symbol table and convert to int
        }
            
        }
        return 0;
    }

    

};


int main(int argc, const char * argv[]) {

    std::string line;
    std::string text;

    // Read program from file
    std::ifstream readFile("main.txt");
    while (std::getline (readFile, line)) {
        text += line;
    }
    readFile.close();
    
    // workaround for xcode (dosen't allow fstream)
    text = "FUNC defineY     y = 5;END FUNC main     x = 7 + 3;    /* 10 */     defineY();    /* Functions currently operate on the global scope  */     z = x + y;    /* 15 */END";



    Parser parser(text);
    FunctionVector* functionVector = parser.functionVector();
    //parser.error(" ");

    // std::cout << typeid(tree).name() << "\n";
    SymbolTable table = SymbolTable();
    table.addFuncsFromFunctionVector(functionVector);

    // std::cout << table;

    Interpreter interpreter = Interpreter(functionVector, table);
    interpreter.visitFunctionVector(functionVector);
    interpreter.printGM();
//       parser.error(" ");
    
    
    // std::cout << "x type = " << EnumToString(table.GLOBAL_SYMBOL_TABLE["x"]);


    for (int i=0; i < functionVector->vector.size(); i++) {

        std::string funcID = functionVector->vector[i]->ID;
        Function* f = functionVector->vector[i];
        
        for (int i=0; i < f->vector->vector.size(); i++) {
            
            std::cout << f->vector->vector[i];
        
        

    }
            
//           std::cout << functionVector->vector[i]->left->token.value << " value = " << parser.visit_expr(tree->vector[i]) << "\n\n";
//           std::cout << functionVector->vector[i];
//           parser.deallocTree(functionVector->vector[i]);
//           std::cout << "\n";
    }
    //parser.error(" ");
    //interpreter.deallocTree(tree);
    std::cout << "\n\n";
    

    



    return 0;

}
