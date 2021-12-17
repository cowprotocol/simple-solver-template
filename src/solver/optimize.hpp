#pragma once

#ifndef OPTIMIZE_H
#define OPTIMIZE_H


class SolvedAuction {
    public:
        bool *executed_order;       // executed_order[i] indicates whether order i was executed or not;
        boost::multiprecision::mpz_int *executed_sell_amount;       // TODO: not memory efficient, revisit.
        boost::multiprecision::mpz_int *executed_buy_amount;

        SolvedAuction (int _num_orders) {
            executed_order = new bool[_num_orders];
            executed_sell_amount = new boost::multiprecision::mpz_int[_num_orders];
            executed_buy_amount = new boost::multiprecision::mpz_int[_num_orders];
        }
};



// The main function that solves the optimization problem and returns the solution, stored in the "solution" vector
void solve_auction(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms);


#endif
