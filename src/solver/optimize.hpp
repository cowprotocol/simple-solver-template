#pragma once

#ifndef OPTIMIZE_H
#define OPTIMIZE_H


class ExecutedOrder {
    public:
        int executed_order_idx;
        int used_amm_idx;
        boost::multiprecision::mpz_int executed_sell_amount;
        boost::multiprecision::mpz_int executed_buy_amount;
        boost::multiprecision::mpf_float buy_token_price;
        boost::multiprecision::mpf_float sell_token_price;
        boost::multiprecision::mpf_float objective_value;

        ExecutedOrder () {
            executed_order_idx = -1;
            used_amm_idx = -1;
            executed_sell_amount = 0;
            executed_buy_amount = 0;
            buy_token_price = 0;
            sell_token_price = 0;
            objective_value = 0;
        }
};



/* 
    The main function that solves the optimization problem and returns the solution, stored in the "solution" vector
    The algorithm is very simple, and is parameterized by an integer parameter k:
        1. It considers all possible subsets of at most k orders;
        2. For each such subset, it considers all possible permutations of its elements;
        3. For each subset and permutation, it considers all possible splits of the permutation into disjoint cycles
        4. Within each cycle, the order imposed by the permutation is respected and the remaining edges of the "cycle" are filled up by AMMs

*/
void solve_auction(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms);


#endif
