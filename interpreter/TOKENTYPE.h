//
//  TOKENTYPE.h
//  interpreter
//
//  Created by Aviv Melamud on 6/24/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#ifndef TOKENTYPE_h
#define TOKENTYPE_h

enum class TOKENTYPE {
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    EMPTY,
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
        case TOKENTYPE::EOL:
            return "EOL";
    }
}

#endif /* TOKENTYPE_h */