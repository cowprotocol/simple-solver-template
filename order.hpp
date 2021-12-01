#pragma once

#ifndef ORDER_H
#define ORDER_H

#include <boost/multiprecision/gmp.hpp>



/***************************** The definition of the class that describes a regular (i.e., trader) order *********************************/
class Order {
    public:
        const int order_index;
        const int sell_token_index;
        const int buy_token_index;
        const bool is_sell_order;
        const bool is_liquidity_order;
        const bool is_partial_fill;
        boost::multiprecision::mpz_int sell_amount;
        boost::multiprecision::mpz_int buy_amount;

        // constructor that 
        Order(int _order_index, int _sell_token_index, int _buy_token_index, bool _is_sell_order, bool _is_liquidity_order, bool _is_partial_fill, boost::multiprecision::mpz_int _sell_amount, boost::multiprecision::mpz_int _buy_amount): order_index(_order_index),  sell_token_index(_sell_token_index), buy_token_index(_buy_token_index), is_sell_order(_is_sell_order), is_liquidity_order(_is_liquidity_order), is_partial_fill(_is_partial_fill), sell_amount(_sell_amount), buy_amount(_buy_amount) {
        }
};
/*******************************************************************************************************************************************/



// An auxiliary function that prints an order in a readable form.
void print_order(Order &o);




#endif