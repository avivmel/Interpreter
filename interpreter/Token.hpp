//
//  Token.hpp
//  interpreter
//
//  Created by Aviv Melamud on 6/24/20.
//  Copyright © 2020 Aviv Melamud. All rights reserved.
//

#ifndef Token_hpp
#define Token_hpp

#include <stdio.h>
#include "TOKENTYPE.h"

class Token {
public:

    TOKENTYPE tokenType = TOKENTYPE::INTEGER;

    std::string value;
    Token() {
        value = ' ';
    };

    Token(TOKENTYPE T, std::string Value) {
         value = Value;
         tokenType = T;
    }

    friend std::ostream& operator<<(std::ostream &os, Token& t) {


        os << "Token(" << EnumToString(t.tokenType) << ", " << t.value << ")";
        return os;
    }

};

#endif /* Token_hpp */
