#pragma once

#ifndef AUX_H
#define AUX_H

#include <vector>
#include "../components/token.hpp"
#include "../components/order.hpp"
#include "../components/amm.hpp"
#include "../../external_libs/json.hpp"

/*
    Function that reads JSON file with batch auction information and builds
    appropriate data structures with all the required information 
*/
void parse_json_file(std::vector<Token> &tokens, std::vector<Order> &orders,
                     std::vector<CP_AMM> &amms, int &_num_tokens,
                     std::map<std::string, int> _idx_tokens,
                     nlohmann::json &json_file);

#endif
