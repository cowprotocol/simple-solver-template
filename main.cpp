#include <iostream>
#include <vector>
#include <fstream>
#include <boost/multiprecision/gmp.hpp>

#include "./json.hpp"
#include "./order.hpp"
#include "./token.hpp"
//#include "./amm.hpp"
#include "./aux.hpp"



/************************************** Global variables that are defined as static in various classes *************************************/

int Token::num_tokens{ 0 };                     // static integer variable of class Token, that counts number of tokens in our instance
std::map<std::string, int> Token::idx_tokens;   // static map of class Token, that maps the address of the token to a unique index

/*******************************************************************************************************************************************/




int main()
{

/********************************************************** VARIABLE DECLARATIONS **********************************************************/

    std::vector<Token> tokens;      // the vector containing all the tokens appearing in any order/AMM
    std::vector<Order> orders;      // the vector containing all the "regular" orders
    //std::vector<AMM> amms;          // the vector containing all the AMMs

/*******************************************************************************************************************************************/


    // Parsing the input
    parse_json_file(tokens, orders, Token::num_tokens, Token::idx_tokens);

    
    // Just printing out the input
    for (auto &i: tokens)
        print_token(i);

    for (auto &i: orders)
        print_order(i, tokens);


    // solve batch auction


    // return solution


    return 0;
}


