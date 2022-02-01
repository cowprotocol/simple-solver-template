#include <iostream>
#include <vector>
#include <fstream>
#include <boost/multiprecision/gmp.hpp>

#include "../../external_libs/json.hpp"
#include "../components/order.hpp"
#include "../components/token.hpp"
#include "../api/aux.hpp"
#include "./optimize.hpp"

/****** Global variables that are defined as static in various classes *******/

// static integer variable of Token class that counts number of tokens
// in our instance
int Token::num_tokens{0};

// static map of class Token, that maps the address of the token to
// a unique index
std::map<std::string, int> Token::idx_tokens;

/*****************************************************************************/

int main()
{

    /************************* VARIABLE DECLARATIONS *************************/

    std::vector<Token> tokens; // vector containing all tokens
    std::vector<Order> orders; // vector containing all orders
    std::vector<CP_AMM> amms;  // vector containing all AMMs

    /*************************************************************************/

    nlohmann::json json_file;
    std::ifstream input_file("./sample_instances/sample.json");
    input_file >> json_file;

    // Parsing the input
    parse_json_file(tokens, orders, amms, Token::num_tokens,
                    Token::idx_tokens, json_file);

    std::cout.precision(std::numeric_limits<boost::multiprecision::
                                                mpf_float>::digits10);

    // solve batch auction
    solve_auction(tokens, orders, amms);

    return 0;
}
