#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include <boost/multiprecision/gmp.hpp>


class Token {
    public:
        static int num_tokens;                                  // counts how many tokens have been created so far
        static std::map<std::string, int> idx_tokens;           // maps the token name to an index
        const std::string name;
        const std::string alias;
        const unsigned int decimals;
        const int idx;                                          // the index of the current token
        const boost::multiprecision::mpf_float external_price;  // external price estimate given as input - if none is provided, then the variable is set to -1.
        const bool normalize_priority;

        // constructor that 
        Token(std::string _name, std::string _alias, unsigned int _decimals, boost::multiprecision::mpf_float _external_price, bool _normalize_priority): name(_name),  alias(_alias), decimals(_decimals), idx(num_tokens++), external_price(_external_price), normalize_priority(_normalize_priority) {
        }
};


// Prints token in a "nice" way
void print_token(Token &t);



#endif

