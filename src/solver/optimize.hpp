#pragma once

#ifndef OPTIMIZE_H
#define OPTIMIZE_H

class ExecutedOrder
{
public:
    int executed_order_idx;
    int used_amm_idx;
    int reference_token_idx;
    boost::multiprecision::mpz_int executed_sell_amount;
    boost::multiprecision::mpz_int executed_buy_amount;
    boost::multiprecision::mpz_int buy_token_price;
    boost::multiprecision::mpz_int sell_token_price;
    boost::multiprecision::mpf_float objective_value;

    ExecutedOrder()
    {
        executed_order_idx = -1;
        used_amm_idx = -1;
        reference_token_idx = -1;
        executed_sell_amount = 0;
        executed_buy_amount = 0;
        buy_token_price = 0;
        sell_token_price = 0;
        objective_value = 0;
    }
};

/* 
    The main function that solves the optimization problem and prints
    the solution. The algorithm is very simple:
        It considers every user fill-or-kill sell/buy order, one by one,
        and attempts to match it against a Uniswap v2 Pool.
        If it succeeds in doing so, it computes the objective value of such
        a settlement, and among all such solutions, it returns the best one.
*/
void solve_auction(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms);

#endif
