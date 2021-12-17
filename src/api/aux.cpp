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
void parse_json_file(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms, int &_num_tokens, std::map<std::string, int> _idx_tokens)
{
    // TODO: Make input reading safe, i.e., handle all potential exceptions/errors
    // TODO: Properly parse JSON and store sell/buy amounts using appropriate data types

    nlohmann::json json_file;
    std::ifstream input_file("../../sample_instances/sample.json");
    input_file >> json_file;

    // For-loop that reads all TOKEN information
    
    for (auto &i: json_file["tokens"].items()) {
        boost::multiprecision::mpf_float t;
        if (i.value()["external_price"].is_null())
            t = -1;
        else {
            //std::string tt = std::to_string(i.value()["external_price"]);
            std::string tt = (i.value()["external_price"]).dump();
            t = static_cast<boost::multiprecision::mpf_float>(tt);
        }
        if (i.value()["alias"].is_null())
            tokens.push_back(Token(i.key(), "_NULL", i.value()["decimals"], t));       // TODO: Check if multiple tokens with "null" alias can create any issue
        else
            tokens.push_back(Token(i.key(), i.value()["alias"], i.value()["decimals"], t));
        _idx_tokens[i.key()] = _num_tokens-1;
    }

    // For-loop that reads all ORDERS information
    for (auto &i: json_file["orders"].items()) {
        int t1 = std::stoi(i.key());
        int t2 = _idx_tokens.at(i.value()["sell_token"]);
        int t3 = _idx_tokens.at(i.value()["buy_token"]);
        bool t4 = i.value()["is_sell_order"];
        bool t5 = i.value()["is_liquidity_order"];
        bool t6 = i.value()["allow_partial_fill"];
        std::string tt7 = i.value()["sell_amount"];
        boost::multiprecision::mpz_int t7 = static_cast<boost::multiprecision::mpz_int>(tt7);
        std::string tt8 = i.value()["buy_amount"];
        boost::multiprecision::mpz_int t8 = static_cast<boost::multiprecision::mpz_int>(tt8);
        int t9 = _idx_tokens.at(i.value()["fee"]["token"]);
        std::string tt10 = i.value()["fee"]["amount"];
        boost::multiprecision::mpz_int t10 = static_cast<boost::multiprecision::mpz_int>(tt10);
        int t11 = _idx_tokens.at(i.value()["cost"]["token"]);
        std::string tt12 = i.value()["cost"]["amount"];
        boost::multiprecision::mpz_int t12 = static_cast<boost::multiprecision::mpz_int>(tt12);

        orders.push_back(Order(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12));
    }

    // For-loop that reads all AMM information
    for (auto &i: json_file["amms"].items()) {
        int t1 = std::stoi(i.key());

        // We will only use ConstantProduct AMMs, so this check ensures that we ignore the rest
        if (i.value()["kind"] != "ConstantProduct")
            continue;
        
        int t2[2];
        boost::multiprecision::mpz_int t3[2];
        int counter = 0;

        for (auto &j: i.value()["reserves"].items()) {
            t2[counter] = _idx_tokens.at(j.key());
            std::string tt3 = j.value();
            t3[counter] = static_cast<boost::multiprecision::mpz_int>(tt3);
            counter++;
        }

        std::string tt4 = i.value()["fee"];
        boost::multiprecision::mpf_float t4 = static_cast<boost::multiprecision::mpf_float>(tt4);
        int t5 = _idx_tokens.at(i.value()["cost"]["token"]);
        std::string tt6 = i.value()["cost"]["amount"];
        boost::multiprecision::mpz_int t6 = static_cast<boost::multiprecision::mpz_int>(tt6);
        bool t7 = i.value()["mandatory"];

        amms.push_back(CP_AMM(t1,t2[0],t3[0],t2[1],t3[1],t4,t5,t6,t7));
    }

    return;
}
/*******************************************************************************************************************************************/

