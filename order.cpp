#include <iostream>
#include "./order.hpp"


// An auxiliary function that prints an order in a readable form.
void print_order(Order &o)
{
    std::cout << "Order index:\t\t" << o.order_index << '\n';
    std::cout << "Sell token index:\t" << o.sell_token_index << '\n';
    std::cout << "Sell amount:\t\t" << o.sell_amount<< '\n';
    std::cout << "Buy token index:\t" << o.buy_token_index << '\n';
    std::cout << "Buy amount:\t\t" << o.buy_amount << '\n';
    std::cout << "Allow partial fill:\t" << (o.is_partial_fill ? "true" : "false") << '\n';
    std::cout << "Is sell order:\t\t" << (o.is_sell_order ? "true" : "false") << '\n';
    std::cout << "Is liquidity order:\t" << (o.is_liquidity_order ? "true" : "false") << "\n\n";
}
