#include <iostream>
#include <vector>
#include <fstream>
#include <boost/multiprecision/gmp.hpp>

#include "../../external_libs/json.hpp"
#include "../components/order.hpp"
#include "../components/token.hpp"
#include "../api/aux.hpp"
#include "./optimize.hpp"



/************************************** Global variables that are defined as static in various classes *************************************/

int Token::num_tokens{ 0 };                     // static integer variable of class Token, that counts number of tokens in our instance
std::map<std::string, int> Token::idx_tokens;   // static map of class Token, that maps the address of the token to a unique index

/*******************************************************************************************************************************************/




int main()
{

/********************************************************** VARIABLE DECLARATIONS **********************************************************/

    std::vector<Token> tokens;      // the vector containing all the tokens appearing in any order/AMM
    std::vector<Order> orders;      // the vector containing all the "regular" orders
    std::vector<CP_AMM> amms;       // the vector containing all the AMMs

/*******************************************************************************************************************************************/

    nlohmann::json json_file;
    std::ifstream input_file("./sample_instances/sample.json");
    input_file >> json_file;

    // Parsing the input
    parse_json_file(tokens, orders, amms, Token::num_tokens, Token::idx_tokens, json_file);

    
    // Just printing out the input
    std::cout.precision(std::numeric_limits<boost::multiprecision::mpf_float>::digits10);

    for (auto &i: tokens)
        print_token(i);

    for (auto &i: orders)
        print_order(i, tokens);

    for (auto &i: amms)
        print_cp_amm(i, tokens);
    

    // solve batch auction
    solve_auction(tokens, orders, amms);

    // return solution


    return 0;
}


