#include <iostream>
#include "./token.hpp"

void print_token(Token &t)
{

    std::cout << "Address:\t" << t.name << '\n';
    std::cout << "Alias:\t\t" << t.alias << '\n';
    std::cout << "Decimals:\t" << t.decimals << '\n';
    std::cout << "External price:\t" << t.external_price << "\n\n";

    return;
}
