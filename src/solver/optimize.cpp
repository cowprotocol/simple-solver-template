#include <math.h>
#include <string>  
#include <iostream> 
#include <sstream> 
#include "../components/token.hpp"
#include "../components/order.hpp"
#include "../components/amm.hpp"
#include "./optimize.hpp"
#include "../../external_libs/json.hpp"


// auxiliary function (used temporarily) for computing x^n using the boost library data types
boost::multiprecision::mpf_float custom_pow(boost::multiprecision::mpz_int x, int n)
{
    int i;
    boost::multiprecision::mpf_float res = 1;
    int m = (n > 0) ? n : (-n);

    for (i = 0; i < m; i++)
        res = res * x;
    if (n > 0)
        return res;
    else
        return (1/res);
}

// main function that processes a sell order, checks if it can be matched against a single liquidity pool
// and if so, it also checks whether it gives a strictly better objective
void process_sell_order(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms, int i, ExecutedOrder &best_order)
{
   int j, num_amms;
    boost::multiprecision::mpz_int x0, y0, x1, y1, x_, y_, x;
    boost::multiprecision::mpf_float float_x1;
    boost::multiprecision::mpf_float float_x;
    boost::multiprecision::mpf_float temp;

    
    x_ = orders[i].buy_amount;
    y_ = orders[i].sell_amount;
    num_amms = amms.size();

    for (j = 0; j < num_amms; j++) {
        x0 = 0;
        y0 = 0;
        if (amms[j].sell_token_index == orders[i].sell_token_index)
            x0 = amms[j].sell_reserve_amount;
        if (amms[j].buy_token_index == orders[i].sell_token_index)
            x0 = amms[j].buy_reserve_amount;
        if (amms[j].sell_token_index == orders[i].buy_token_index)
            y0 = amms[j].sell_reserve_amount;
        if (amms[j].buy_token_index == orders[i].buy_token_index)
            y0 = amms[j].buy_reserve_amount;

        if ((x0 <= 0) || (y0 <= 0))
            continue;

        // at this point, we have found a "matching" AMM
        // we must satisfy x1 * y1 >= x0 * y0, where (x1,y1) is the new pool balance and (x0, y0) is the original pool balance
        // Since it is a fill-or-kill sell order, we have x1 = x0 + (1-fee)y_
        // and y1 = y0 - x
        // Thus, maximally exploiting the pool, we get that y1 = x0 * y0 / x1,
        // which implies that x = y0 * (1 - x0/x1)
        float_x1 = static_cast<boost::multiprecision::mpf_float>(x0) + (1 - amms[j].fee_amount) * static_cast<boost::multiprecision::mpf_float>(y_);
        float_x = y0 * (1 - x0/float_x1);
        x = static_cast<boost::multiprecision::mpz_int>(float_x);

        if (x < x_)
            continue;
        
        // at this point, the trade can happen and so we now check if objective improves, and if so, compute the prices
        temp = (x - x_) * tokens[orders[i].buy_token_index].external_price  + orders[i].fee_amount * tokens[orders[i].fee_token_index].external_price - orders[i].cost_amount * tokens[orders[i].cost_token_index].external_price;

        if (temp > best_order.objective_value) {
            // found better solution
            best_order.objective_value = temp;
            best_order.executed_order_idx = i;
            best_order.used_amm_idx = j;
            best_order.executed_buy_amount = x;
            best_order.executed_sell_amount = y_;
            if (tokens[orders[i].buy_token_index].normalize_priority) {
                best_order.buy_token_price = 1.0;
                best_order.sell_token_price = (static_cast<boost::multiprecision::mpf_float>(x)) / (static_cast<boost::multiprecision::mpf_float>(y_));

            }
            else {
                best_order.sell_token_price = 1.0;
                best_order.buy_token_price = (static_cast<boost::multiprecision::mpf_float>(y_)) / (static_cast<boost::multiprecision::mpf_float>(x));
            }
            best_order.buy_token_price = best_order.buy_token_price * pow(10, tokens[orders[i].buy_token_index].decimals);
            best_order.sell_token_price = best_order.sell_token_price * pow(10, tokens[orders[i].sell_token_index].decimals);
        }
    }
    return;
}



// main function that processes a buy order, checks if it can be matched against a single liquidity pool
// and if so, it also checks whether it gives a strictly better objective
void process_buy_order(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms, int i, ExecutedOrder &best_order)
{
    int j, num_amms;
    boost::multiprecision::mpz_int x0, y0, x1, y1, x_, y_, y;
    boost::multiprecision::mpf_float float_y1;
    boost::multiprecision::mpf_float float_y;
    boost::multiprecision::mpf_float temp;
    
    x_ = orders[i].buy_amount;
    y_ = orders[i].sell_amount;
    num_amms = amms.size();

    for (j = 0; j < num_amms; j++) {
        x0 = 0;
        y0 = 0;
        if (amms[j].sell_token_index == orders[i].sell_token_index)
            x0 = amms[j].sell_reserve_amount;
        if (amms[j].buy_token_index == orders[i].sell_token_index)
            x0 = amms[j].buy_reserve_amount;
        if (amms[j].sell_token_index == orders[i].buy_token_index)
            y0 = amms[j].sell_reserve_amount;
        if (amms[j].buy_token_index == orders[i].buy_token_index)
            y0 = amms[j].buy_reserve_amount;

        if ((x0 <= 0) || (y0 <= x_))
            continue;

        // at this point, we have found a "matching" AMM
        // we must satisfy x1 * y1 >= x0 * y0, where (x1,y1) is the new pool balance and (x0, y0) is the original pool balance
        // Since it is a fill-or-kill sell order, we have x1 = x0 + (1-fee)y
        // and y1 = y0 - x_
        // Thus, maximally exploiting the pool, we get that x1 = x0 * y0 / y1,
        // which implies that y = ((y0 * x0 / y1) - x0)  / (1-fee);
        float_y1 = static_cast<boost::multiprecision::mpf_float>(y0 - x_);
        float_y = (( static_cast<boost::multiprecision::mpf_float>(y0 * x0) / float_y1) - static_cast<boost::multiprecision::mpf_float>(x0)) / (1 - amms[j].fee_amount);
        y = static_cast<boost::multiprecision::mpz_int>(float_y);

        if (y > y_)
            continue;
        
        // at this point, the trade can happen and so we now check if objective improves, and if so, compute the prices
        temp = (y_ - y) * tokens[orders[i].sell_token_index].external_price + orders[i].fee_amount * tokens[orders[i].fee_token_index].external_price - orders[i].cost_amount * tokens[orders[i].cost_token_index].external_price;

        if (temp > best_order.objective_value) {
            // found better solution
            best_order.objective_value = temp;
            best_order.executed_order_idx = i;
            best_order.used_amm_idx = j;
            best_order.executed_buy_amount = x_;
            best_order.executed_sell_amount = y;
            if (tokens[orders[i].buy_token_index].normalize_priority) {
                best_order.buy_token_price = 1.0;
                best_order.sell_token_price = (static_cast<boost::multiprecision::mpf_float>(x_)) / (static_cast<boost::multiprecision::mpf_float>(y));
            }
            else {
                best_order.sell_token_price = 1.0;
                best_order.buy_token_price = (static_cast<boost::multiprecision::mpf_float>(y)) / (static_cast<boost::multiprecision::mpf_float>(x_));
            }
            best_order.buy_token_price = best_order.buy_token_price * pow(10, tokens[orders[i].buy_token_index].decimals);
            best_order.sell_token_price = best_order.sell_token_price * pow(10, tokens[orders[i].sell_token_index].decimals);
        }
    }
    return;
}



void generate_output(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms, ExecutedOrder &best_order, nlohmann::json &output)
{
    bool found;
    boost::multiprecision::mpz_int temp;

    found = (best_order.objective_value > 0) ? true : false;

    output["amms"];
    output["metadata"];
    output["orders"];
    output["prices"];
    output["ref_token"];
    output["tokens"];

    output["metadata"]["has_solution"] = true;
    output["metadata"]["result"] = "xxxx";

    for (long unsigned int i = 0; i < tokens.size(); i++)
        if (tokens[i].normalize_priority)
            output["ref_token"] = tokens[i].name;

    if (found) {

        // Generate "amms" portion of output json
        int amm_idx = amms[best_order.used_amm_idx].amm_index;
        std::string i = std::to_string(amm_idx);

        nlohmann::json cost_entry;
        cost_entry["amount"] = boost::lexical_cast<std::string>(amms[amm_idx].cost_amount);
        cost_entry["token"] = tokens[amms[amm_idx].cost_token_index].name;
        output["amms"][i]["cost"] = cost_entry;

        nlohmann::json execution_entry_contents;
        nlohmann::json exec_plan;
        execution_entry_contents["buy_token"] = tokens[amms[amm_idx].buy_token_index].name;
        execution_entry_contents["sell_token"] = tokens[amms[amm_idx].sell_token_index].name;
        execution_entry_contents["exec_buy_amount"] = "xxx";
        execution_entry_contents["exec_sell_amount"] = "yyy";
        
        exec_plan["position"] = 0;
        exec_plan["sequence"] = 0;
        execution_entry_contents["exec_plan"] = exec_plan;
        nlohmann::json execution_entry = { execution_entry_contents };
        output["amms"][i]["execution"] = execution_entry;

        output["amms"]["fee"] = boost::lexical_cast<std::string>(amms[amm_idx].fee_amount);
        output["amms"]["kind"] = "ConstantProduct";
        output["amms"]["mandatory"] = "false";

        nlohmann::json reserves;
        reserves[tokens[amms[amm_idx].sell_token_index].name] = "asd";
        reserves[tokens[amms[amm_idx].buy_token_index].name] = "asd";
        output["amms"]["reserves"] = reserves;


        // Generate "orders" portion of output json
        int order_idx = orders[best_order.executed_order_idx].order_index;
        std::string j = std::to_string(order_idx);
        output["orders"][j];
        output["orders"][j]["allow_partial_fill"] = false;
        output["orders"][j]["buy_amount"] = boost::lexical_cast<std::string>(orders[order_idx].buy_amount);
        output["orders"][j]["buy_token"] = tokens[orders[order_idx].buy_token_index].name;
        output["orders"][j]["cost"]["amount"] = boost::lexical_cast<std::string>(orders[order_idx].cost_amount);
        output["orders"][j]["cost"]["token"] = tokens[orders[order_idx].cost_token_index].name;
        output["orders"][j]["exec_buy_amount"] = boost::lexical_cast<std::string>(best_order.executed_buy_amount);
        output["orders"][j]["exec_sell_amount"] = boost::lexical_cast<std::string>(best_order.executed_sell_amount);
        output["orders"][j]["fee"]["amount"] = boost::lexical_cast<std::string>(orders[order_idx].fee_amount);
        output["orders"][j]["fee"]["token"] = tokens[orders[order_idx].fee_token_index].name;
        output["orders"][j]["is_liquidity_order"] = orders[order_idx].is_liquidity_order;
        output["orders"][j]["is_sell_order"] = orders[order_idx].is_sell_order;
        output["orders"][j]["mandatory"] = false;
        output["orders"][j]["sell_amount"] = boost::lexical_cast<std::string>(orders[order_idx].sell_amount);
        output["orders"][j]["sell_token"] = tokens[orders[order_idx].sell_token_index].name;

        // Generate "prices" section of output json file
        auto sell_token_idx = orders[order_idx].sell_token_index;
        auto buy_token_idx = orders[order_idx].buy_token_index;
        std::stringstream buffer, buffer2;
        buffer.precision(std::numeric_limits<boost::multiprecision::mpf_float>::digits10);
        buffer << best_order.sell_token_price;
        buffer2.precision(std::numeric_limits<boost::multiprecision::mpf_float>::digits10);
        buffer2 << best_order.buy_token_price;
        output["prices"][tokens[sell_token_idx].name] = buffer.str();//boost::lexical_cast<std::string>(best_order.sell_token_price);
        output["prices"][tokens[buy_token_idx].name] = buffer2.str();//boost::lexical_cast<std::string>(best_order.buy_token_price);

        // Generate "tokens" section of output json file
        output["tokens"][tokens[sell_token_idx].name]["alias"] = tokens[sell_token_idx].alias;
        output["tokens"][tokens[sell_token_idx].name]["decimals"] = tokens[sell_token_idx].decimals;
        output["tokens"][tokens[buy_token_idx].name]["alias"] = tokens[buy_token_idx].alias;
        output["tokens"][tokens[buy_token_idx].name]["decimals"] = tokens[buy_token_idx].decimals;

    }

    return;
}





/*********************************************************************************************************************************************/
void solve_auction(std::vector<Token> &tokens, std::vector<Order> &orders, std::vector<CP_AMM> &amms)
{
    int i;
    int num_orders;
    nlohmann::json output;

    ExecutedOrder best_order;
    num_orders = orders.size();

    // We only consider fill-or-kill user orders, one by one
    for (i = 0; i < num_orders; i++) {
        if (orders[i].is_partial_fill)
            continue;
        if (orders[i].is_liquidity_order)
            continue;

        if (orders[i].is_sell_order)
            process_sell_order(tokens, orders, amms, i, best_order);
        else
            process_buy_order(tokens, orders, amms, i, best_order);
    }

    generate_output(tokens, orders, amms, best_order, output);
    std::cout << output.dump(4) << "\n";
    return;
}



