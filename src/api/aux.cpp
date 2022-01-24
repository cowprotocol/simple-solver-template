/*
    This file contains some auxiliary functions
*/

#include <iostream>
#include <fstream>
#include <string>
#include <boost/multiprecision/gmp.hpp>

#include "./aux.hpp"


#define DEMANGLE_TYPEID_NAME(x) abi::__cxa_demangle(typeid((x)).name(), NULL, NULL, NULL)


/*** Function that reads JSON file with batch auction information and builds appropriate data structures with all the required information **/
void parse_json_file(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms, int &_num_tokens, std::map<std::string, int> _idx_tokens, nlohmann::json &json_file)
{
    // TODO: Make input reading safe, i.e., handle all potential exceptions/errors

    // For-loop that reads all TOKEN information
    for (auto &tkn: json_file["tokens"].items()) {
        boost::multiprecision::mpf_float external_price;
        bool normalize_priority;

        normalize_priority = (tkn.value()["normalize_priority"] == 1);
        if (!tkn.value()["external_price"].is_null()) {
            std::string external_price_string = (tkn.value()["external_price"]).dump();
            external_price = static_cast<boost::multiprecision::mpf_float>(external_price_string);
        }
        else
            external_price = -1;

        if (!tkn.value()["alias"].is_null())
            tokens.push_back(Token(tkn.key(), tkn.value()["alias"], tkn.value()["decimals"], external_price, normalize_priority));
        else
            tokens.push_back(Token(tkn.key(), "_NULL", tkn.value()["decimals"], external_price, normalize_priority));       // TODO: Check if multiple tokens with "null" alias can create any issue
            
        _idx_tokens[tkn.key()] = _num_tokens-1;
    }

    // For-loop that reads all ORDERS information
    for (auto &o: json_file["orders"].items()) {
        int key = std::stoi(o.key());
        int sell_token = _idx_tokens.at(o.value()["sell_token"]);
        int buy_token = _idx_tokens.at(o.value()["buy_token"]);
        bool is_sell_order = o.value()["is_sell_order"];
        bool is_liquidity_order = o.value()["is_liquidity_order"];
        bool allow_partial_fill = o.value()["allow_partial_fill"];
        std::string sell_amount_string = o.value()["sell_amount"];
        boost::multiprecision::mpz_int sell_amount = static_cast<boost::multiprecision::mpz_int>(sell_amount_string);
        std::string buy_amount_string = o.value()["buy_amount"];
        boost::multiprecision::mpz_int buy_amount = static_cast<boost::multiprecision::mpz_int>(buy_amount_string);
        int fee_token = _idx_tokens.at(o.value()["fee"]["token"]);
        std::string fee_amount_string = o.value()["fee"]["amount"];
        boost::multiprecision::mpz_int fee_amount = static_cast<boost::multiprecision::mpz_int>(fee_amount_string);
        int cost_token = _idx_tokens.at(o.value()["cost"]["token"]);
        std::string cost_amount_string = o.value()["cost"]["amount"];
        boost::multiprecision::mpz_int cost_amount = static_cast<boost::multiprecision::mpz_int>(cost_amount_string);

        orders.push_back(Order(key, sell_token, buy_token, is_sell_order, is_liquidity_order, allow_partial_fill, sell_amount, buy_amount, fee_token, fee_amount, cost_token, cost_amount));
    }

    // For-loop that reads all AMM information
    for (auto &am: json_file["amms"].items()) {
        int key = std::stoi(am.key());

        // We will only use ConstantProduct AMMs, so this check ensures that we ignore the rest
        if (am.value()["kind"] != "ConstantProduct")
            continue;
        
        int pools_idx[2];
        boost::multiprecision::mpz_int pools_reserves[2];
        int counter = 0;

        for (auto &j: am.value()["reserves"].items()) {
            pools_idx[counter] = _idx_tokens.at(j.key());
            std::string reserves_string = j.value();
            pools_reserves[counter] = static_cast<boost::multiprecision::mpz_int>(reserves_string);
            counter++;
        }

        std::string fee_fraction_string = am.value()["fee"];
        boost::multiprecision::mpf_float fee_fraction = static_cast<boost::multiprecision::mpf_float>(fee_fraction_string);
        int cost_token = _idx_tokens.at(am.value()["cost"]["token"]);
        std::string cost_amount_string = am.value()["cost"]["amount"];
        boost::multiprecision::mpz_int cost_amount = static_cast<boost::multiprecision::mpz_int>(cost_amount_string);
        bool is_mandatory = am.value()["mandatory"];

        amms.push_back(CP_AMM(key, pools_idx[0], pools_reserves[0], pools_idx[1], pools_reserves[1], fee_fraction, cost_token, cost_amount, is_mandatory));
    }

    return;
}
/*******************************************************************************************************************************************/

