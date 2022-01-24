#include <iostream>
#include <vector>
#include "./amm.hpp"
#include "./token.hpp"




// An auxiliary function that prints a CP AMM in a readable form.
void print_cp_amm(CP_AMM &a, std::vector<Token> &_tokens)
{
    std::cout << "AMM index:\t\t\t" << a.amm_index << '\n';
    std::cout << "Sell token index/alias:\t\t" << a.sell_token_index << " (" << _tokens[a.sell_token_index].alias << ')' << '\n';
    std::cout << "Sell amount:\t\t\t" << a.sell_reserve_amount<< '\n';
    std::cout << "Buy token index/alias:\t\t" << a.buy_token_index << " (" << _tokens[a.buy_token_index].alias << ')' << '\n';
    std::cout << "Buy amount:\t\t\t" << a.buy_reserve_amount << '\n';
    std::cout << "Fee fraction:\t\t\t" << a.fee_fraction << '\n';
    std::cout << "Cost:\n";
    std::cout << "    Cost token index/alias:\t" << a.cost_token_index << " (" << _tokens[a.cost_token_index].alias << ')' << '\n';
    std::cout << "    Cost amount:\t\t" << a.cost_amount << '\n';
    std::cout << "Is mandatory:\t\t\t" << (a.is_mandatory ? "true" : "false") << "\n\n";
}
