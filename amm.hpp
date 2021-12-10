#pragma once

#ifndef AMM_H
#define AMM_H

#include <boost/multiprecision/gmp.hpp>
#include "./token.hpp"


// sample class describing a simple constant product AMM
// We use the convention that the first token is the sell token
// and the second token is the buy token.
class CP_AMM {
    public:
        const int amm_index;
        const int sell_token_index;
        const boost::multiprecision::mpz_int sell_reserve_amount;
        const int buy_token_index;
        const boost::multiprecision::mpz_int buy_reserve_amount;
        const boost::multiprecision::mpf_float fee_amount;
        const int cost_token_index;
        const boost::multiprecision::mpz_int cost_amount;
        const bool is_mandatory;



        // Constructor
        CP_AMM(int _amm_index, int _sell_token_index, boost::multiprecision::mpz_int _sell_reserve_amount, int _buy_token_index, boost::multiprecision::mpz_int _buy_reserve_amount, boost::multiprecision::mpf_float _fee_amount, int _cost_token_index, boost::multiprecision::mpz_int _cost_amount, bool _is_mandatory): amm_index(_amm_index), sell_token_index(_sell_token_index),  sell_reserve_amount(_sell_reserve_amount), buy_token_index(_buy_token_index), buy_reserve_amount(_buy_reserve_amount), fee_amount(_fee_amount), cost_token_index(_cost_token_index), cost_amount(_cost_amount), is_mandatory(_is_mandatory) {
        }

};
        

void print_cp_amm(CP_AMM &a, std::vector<Token> &_tokens);



#endif