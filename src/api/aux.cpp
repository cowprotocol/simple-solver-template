/*
    This file contains some auxiliary functions
*/

#include <iostream>
#include <fstream>
#include <string>
#include <boost/multiprecision/gmp.hpp>
#include "./aux.hpp"


boost::multiprecision::mpz_int parse_string_to_mpz_int(std::string const& input_string)
{
    boost::multiprecision::mpz_int res = static_cast<boost::multiprecision::mpz_int>(input_string);
    return res;
}

boost::multiprecision::mpf_float parse_string_to_mpf_float(std::string const& input_string)
{
    boost::multiprecision::mpf_float res = static_cast<boost::multiprecision::mpf_float>(input_string);
    return res;
}



/*** Function that reads JSON file with batch auction information and builds appropriate data structures with all the required information **/
void parse_json_file(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms, int &_num_tokens, std::map<std::string, int> _idx_tokens, nlohmann::json &json_file)
{
    // TODO: Make input reading safe, i.e., handle all potential exceptions/errors

    // For-loop that reads all TOKEN information
    for (auto const& tkn: json_file["tokens"].items()) {
        boost::multiprecision::mpf_float external_price;
        bool const normalize_priority = (tkn.value()["normalize_priority"] == 1);

        if (!tkn.value()["external_price"].is_null())
            external_price = parse_string_to_mpf_float((tkn.value()["external_price"]).dump());
        else
            external_price = -1;

        if (!tkn.value()["alias"].is_null())
            tokens.push_back(Token(tkn.key(), tkn.value()["alias"], tkn.value()["decimals"], external_price, normalize_priority));
        else
            tokens.push_back(Token(tkn.key(), "_NULL", tkn.value()["decimals"], external_price, normalize_priority));       // TODO: Check if multiple tokens with "null" alias can create any issue
            
        _idx_tokens[tkn.key()] = _num_tokens-1;
    }

    // For-loop that reads all ORDERS information
    for (auto const& o: json_file["orders"].items()) {
        int const key = std::stoi(o.key());
        int const sell_token = _idx_tokens.at(o.value()["sell_token"]);
        int const buy_token = _idx_tokens.at(o.value()["buy_token"]);
        bool const is_sell_order = o.value()["is_sell_order"];
        bool const is_liquidity_order = o.value()["is_liquidity_order"];
        bool const allow_partial_fill = o.value()["allow_partial_fill"];
        boost::multiprecision::mpz_int const sell_amount = parse_string_to_mpz_int(o.value()["sell_amount"]);
        boost::multiprecision::mpz_int const buy_amount = parse_string_to_mpz_int(o.value()["buy_amount"]);
        int const fee_token = _idx_tokens.at(o.value()["fee"]["token"]);
        boost::multiprecision::mpz_int const fee_amount = parse_string_to_mpz_int(o.value()["fee"]["amount"]);
        int const cost_token = _idx_tokens.at(o.value()["cost"]["token"]);
        boost::multiprecision::mpz_int const cost_amount = parse_string_to_mpz_int(o.value()["cost"]["amount"]);

        orders.push_back(Order(key, sell_token, buy_token, is_sell_order, is_liquidity_order, allow_partial_fill, sell_amount, buy_amount, fee_token, fee_amount, cost_token, cost_amount));
    }

    // For-loop that reads all AMM information
    for (auto const& am: json_file["amms"].items()) {
        int const key = std::stoi(am.key());

        // We will only use ConstantProduct AMMs, so this check ensures that we ignore the rest
        if (am.value()["kind"] != "ConstantProduct")
            continue;
        
        int pools_idx[2];
        boost::multiprecision::mpz_int pools_reserves[2];
        std::size_t counter = 0;

        for (auto &pool_tokens: am.value()["reserves"].items()) {
            pools_idx[counter] = _idx_tokens.at(pool_tokens.key());
            pools_reserves[counter] =  parse_string_to_mpz_int(pool_tokens.value());
            counter++;
        }

        boost::multiprecision::mpf_float const fee_fraction = parse_string_to_mpf_float(am.value()["fee"]);
        int const cost_token = _idx_tokens.at(am.value()["cost"]["token"]);
        boost::multiprecision::mpz_int const cost_amount = parse_string_to_mpz_int(am.value()["cost"]["amount"]);
        bool const is_mandatory = am.value()["mandatory"];

        amms.push_back(CP_AMM(key, pools_idx[0], pools_reserves[0], pools_idx[1], pools_reserves[1], fee_fraction, cost_token, cost_amount, is_mandatory));
    }

    return;
}
/*******************************************************************************************************************************************/

