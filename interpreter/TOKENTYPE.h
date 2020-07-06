//
//  TOKENTYPE.h
//  interpreter
//
//  Created by Aviv Melamud on 6/24/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#include <iostream>

#ifndef TOKENTYPE_h
#define TOKENTYPE_h

/*
 Defines the type of data a token is storing
 */
enum class TOKENTYPE {
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    EMPTY,
    ASSIGN,
    ID, // name of a variable
    SEMI,
    FUNC,
    END,
    VOID,
    FUNCCALL,
    EOL
};


std::string EnumToString(TOKENTYPE tokenType) {

    switch (tokenType) {
        case TOKENTYPE::INTEGER:
            return "INTEGER";

        case TOKENTYPE::PLUS:
            return "PLUS";
        case TOKENTYPE::MINUS:
            return "MINUS";

        case TOKENTYPE::MUL:
            return "MUL";
        case TOKENTYPE::DIV:
            return "DIV";

        case TOKENTYPE::LPAREN:
            return "LPAREN";
        case TOKENTYPE::RPAREN:
            return "RPAREN";

        case TOKENTYPE::EMPTY:
            return "EMPTY";

        case TOKENTYPE::ASSIGN:
            return "ASSIGN";
        case TOKENTYPE::ID:
            return "ID";

        case TOKENTYPE::SEMI:
            return "SEMI";
        case TOKENTYPE::FUNC:
            return "FUNC";
        case TOKENTYPE::END:
            return "END";
        case TOKENTYPE::VOID:
            return "VOID";
        case TOKENTYPE::FUNCCALL:
            return "FUNCCALL";


        case TOKENTYPE::EOL:
            return "EOL";
    }
}

#endif /* TOKENTYPE_h */
