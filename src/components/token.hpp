#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include <boost/multiprecision/gmp.hpp>

/************ The definition of the class that describes a token *************/
class Token
{
public:
    // number of tokens that have been created
    static int num_tokens;

    // maps the token name to an index
    static std::map<std::string, int> idx_tokens;

    const std::string name;
    const std::string alias;
    const unsigned int decimals;

    // the index of the current token
    const int idx;

    // external input price estimate; if none is provided then variable is set to -1
    const boost::multiprecision::mpf_float external_price;
    const bool normalize_priority;

    // constructor that
    Token(std::string _name, std::string _alias, unsigned int _decimals,
          boost::multiprecision::mpf_float _external_price,
          bool _normalize_priority) : name(_name), alias(_alias),
                                      decimals(_decimals), idx(num_tokens++),
                                      external_price(_external_price),
                                      normalize_priority(_normalize_priority)
    {
    }
};
/*****************************************************************************/

// Prints token in a "nice" way
void print_token(Token &t);

#endif
