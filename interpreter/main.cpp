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
#include "Parser.h"
#include "SymbolTable.h"




int main(int argc, const char * argv[]) {



   while (true) {
       std::cout << ">> ";
       
       std::string text;
       getline(std::cin, text);

       Parser parser(text);
       statementsVector* tree = parser.statements_list();
       // std::cout << typeid(tree).name() << "\n";
       SymbolTable table = SymbolTable();
       table.addVars(tree);
       
       // std::cout << "x type = " << EnumToString(table.GLOBAL_SYMBOL_TABLE["x"]);


       for (int i=0; i < tree->vector.size(); i++) {
           std::cout << tree->vector[i]->left->token.value << " value = " << parser.visit_expr(tree->vector[i]) << "\n\n";
           std::cout << tree->vector[i];
           parser.deallocTree(tree->vector[i]);
           std::cout << "\n";
       }
       //interpreter.error(" ");
       //interpreter.deallocTree(tree);
       std::cout << "\n\n";
       //interpreter.error();
   }

    

    



    return 0;

}
