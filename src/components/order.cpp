#include <iostream>
#include <vector>
#include "./order.hpp"
#include "./token.hpp"

// An auxiliary function that prints an order in a readable form.
void print_order(Order &o, std::vector<Token> &_tokens)
{
    std::cout << "Order index:\t\t\t" << o.order_index << '\n';
    std::cout << "Sell token index/alias:\t\t" << o.sell_token_index << " (" << _tokens[o.sell_token_index].alias << ')' << '\n';
    std::cout << "Sell amount:\t\t\t" << o.sell_amount << '\n';
    std::cout << "Buy token index/alias:\t\t" << o.buy_token_index << " (" << _tokens[o.buy_token_index].alias << ')' << '\n';
    std::cout << "Buy amount:\t\t\t" << o.buy_amount << '\n';
    std::cout << "Allow partial fill:\t\t" << (o.is_partial_fill ? "true" : "false") << '\n';
    std::cout << "Is sell order:\t\t\t" << (o.is_sell_order ? "true" : "false") << '\n';
    std::cout << "Is liquidity order:\t\t" << (o.is_liquidity_order ? "true" : "false") << '\n';
    std::cout << "Fee:\n";
    std::cout << "    Fee token index/alias:\t" << o.fee_token_index << " (" << _tokens[o.fee_token_index].alias << ')' << '\n';
    std::cout << "    Fee amount:\t\t\t" << o.fee_amount << '\n';
    std::cout << "Cost:\n";
    std::cout << "    Cost token index/alias:\t" << o.cost_token_index << " (" << _tokens[o.cost_token_index].alias << ')' << '\n';
    std::cout << "    Cost amount:\t\t" << o.cost_amount << "\n\n";
}
