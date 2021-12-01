/*
    This file contains some auxiliary functions
*/

#include <iostream>
#include <fstream>
#include "./aux.hpp"



/*** Function that reads JSON file with batch auction information and build appropriate data structures with all the required information **/
void parse_json_file(std::vector<Token> &tokens, std::vector<Order> &orders, int &_num_tokens, std::map<std::string, int> _idx_tokens)
{
    // TODO: Make input reading safe, i.e., handle all potential exceptions/errors
    // TODO: Properly parse JSON and store sell/buy amounts using appropriate data types

    nlohmann::json json_file;
    std::ifstream input_file("./sample.json");
    input_file >> json_file;

    // For-loop that reads all token information
    for (auto &i: json_file["tokens"].items()) {
        if (i.value()["alias"].is_null())
            tokens.push_back(Token(i.key(), "_NULL", i.value()["decimals"]));       // TODO: Check if multiple tokens with "null" alias can create any issue
        else
            tokens.push_back(Token(i.key(), i.value()["alias"], i.value()["decimals"]));
        _idx_tokens[i.key()] = _num_tokens-1;
    }

    // For-loop that reads all order information
    for (auto &i: json_file["orders"].items()) {
        int t1 = std::stoi(i.key());
        int t2 = _idx_tokens.at(i.value()["sell_token"]);
        int t3 = _idx_tokens.at(i.value()["buy_token"]);
        bool t4 = i.value()["is_sell_order"];
        bool t5 = i.value()["is_liquidity_order"];
        bool t6 = i.value()["allow_partial_fill"];
        orders.push_back(Order(t1,t2,t3,t4,t5,t6));
    }

    return;
}
/*******************************************************************************************************************************************/

