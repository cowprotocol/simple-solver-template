#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

class Token {
    public:
        static int num_tokens;                              // counts how many tokens have been created so far
        static std::map<std::string, int> idx_tokens;       // maps the token name to an index
        const std::string name;
        const std::string alias;
        const unsigned int decimals;
        const int idx;                                      // the index of the current token


        // constructor that 
        Token(std::string _name, std::string _alias, unsigned int _decimals): name(_name),  alias(_alias), decimals(_decimals), idx(num_tokens++) {
        }
};


// Prints token t in a "nice" way
void print_token(Token &t);



#endif

