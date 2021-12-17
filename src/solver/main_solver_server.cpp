#include <iostream>
#include <vector>
#include <fstream>
#include <boost/multiprecision/gmp.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include "../../external_libs/httplib.hpp"
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


    httplib::Server server;

    // Register endpoints

    server.Get("/health", [](const httplib::Request&, httplib::Response &res) {
        res.status = 200;
        return true;
    });

    server.Post("/solve", [&](const httplib::Request& req, httplib::Response &res) {
        auto start_time = std::chrono::steady_clock::now();
        spdlog::stopwatch sw;
        spdlog::info("\n\n--- 8< ------ 8< ------ 8< ------ 8< ------ 8< ------ 8< ------ 8< --- 8< --- 8< --- 8< ---\n");
        spdlog::info("Processing new /solve request ...");
        spdlog::info("Received instance (raw form):\n{}\n", req.body);
   
        
        // Parsing the input
        parse_json_file(tokens, orders, amms, Token::num_tokens, Token::idx_tokens, true, req.body);

    
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

        std::istringstream is(req.body); 
        std::ostringstream os;
        res.body = os.str();
        res.status = 200;

        return true;
    });

    // Set error handler

    server.set_exception_handler([&](const auto& req, auto& res, std::exception &e) {
        res.status = 500;
        spdlog::error(
            "Exception raised serving {}:\n{}",
            req.path,
            e.what()
        );
    });

    // Set logger

    server.set_logger([](const auto& req, const auto& res) {
        if (req.path == "/health")
            return; // to avoid spamming the logger.

        spdlog::info(
            "Processed {} request to {} with status {}.",
            req.method,
            req.path,
            res.status
        );
    });

    // Set host and port from env vars

    std::string host = "0.0.0.0";
    std::size_t port = 8000;

    if (const char* p_host_name = std::getenv("HOST"))
        host = std::string(p_host_name);

    if (const char* p_port = std::getenv("PORT"))
        port = atoi(p_port);

    spdlog::info(
        "Starting HTTP server ({} threads) on {}:{} ...",
        CPPHTTPLIB_THREAD_POOL_COUNT,
        host,
        port
    );
    server.listen(host.c_str(), port);





    return 0;
}


