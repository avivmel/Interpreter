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
       FunctionVector* functionVector = parser.functionVector();
       //parser.error(" ");

       // std::cout << typeid(tree).name() << "\n";
       SymbolTable table = SymbolTable();
       table.AddFuncsFromFunctionVector(functionVector);

       std::cout << table;
       
       // std::cout << "x type = " << EnumToString(table.GLOBAL_SYMBOL_TABLE["x"]);


//       for (int i=0; i < functionVector->vector.size(); i++) {
//           std::cout << functionVector->vector[i]->left->token.value << " value = " << parser.visit_expr(tree->vector[i]) << "\n\n";
//           std::cout << functionVector->vector[i];
//           parser.deallocTree(functionVector->vector[i]);
//           std::cout << "\n";
//       }
       //parser.error(" ");
       //interpreter.deallocTree(tree);
       std::cout << "\n\n";
   }

    

    



    return 0;

}
