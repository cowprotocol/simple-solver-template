#pragma once

#ifndef AUX_H
#define AUX_H

#include <vector>
#include "./token.hpp"
#include "./order.hpp"
#include "./json.hpp"


/*** Function that reads JSON file with batch auction information and build appropriate data structures with all the required information **/
void parse_json_file(std::vector<Token> &tokens, std::vector<Order> &orders, int &_num_tokens, std::map<std::string, int> _idx_tokens);


#endif

