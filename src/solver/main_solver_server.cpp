#include <iostream>
#include <vector>
#include <fstream>
#include <boost/multiprecision/gmp.hpp>

#include <spdlog/spdlog.h>
#include "../../external_libs/httplib.hpp"
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
    httplib::Server server;

    // Register endpoints

    server.Get("/health", [](const httplib::Request &, httplib::Response &res)
               {
                   res.status = 200;
                   return true;
               });

    server.Post("/solve", [&](const httplib::Request &req,
                              httplib::Response &res)
                {
                    spdlog::info("\n\n--- 8< ------ 8< ------ 8< ------ 8< \
                    ------ 8< ------ 8< ------ 8< --- 8< --- 8< --- 8< ---\n");
                    spdlog::info("Processing new /solve request ...");
                    spdlog::info("Received instance (raw form):\n{}\n",
                                 req.body);

                    json_file = nlohmann::json::parse(req.body);
                    // Parsing the inputc++ how many character per line
                    parse_json_file(tokens, orders, amms, Token::num_tokens,
                                    Token::idx_tokens, json_file);

                    std::cout.precision(std::numeric_limits<
                                        boost::multiprecision::mpf_float>::digits10);

                    // solve batch auction
                    solve_auction(tokens, orders, amms);

                    std::istringstream is(req.body);
                    std::ostringstream os;
                    res.body = os.str();
                    res.status = 200;

                    return true;
                });

    // Set error handler

    server.set_exception_handler([&](const auto &req, auto &res,
                                     std::exception &e)
                                 {
                                     res.status = 500;
                                     spdlog::error(
                                         "Exception raised serving {}:\n{}",
                                         req.path,
                                         e.what());
                                 });

    // Set logger

    server.set_logger([](const auto &req, const auto &res)
                      {
                          if (req.path == "/health")
                              return; // to avoid spamming the logger.

                          spdlog::info(
                              "Processed {} request to {} with status {}.",
                              req.method,
                              req.path,
                              res.status);
                      });

    // Set host and port from env vars

    std::string host = "0.0.0.0";
    std::size_t port = 8000;

    if (const char *p_host_name = std::getenv("HOST"))
        host = std::string(p_host_name);

    if (const char *p_port = std::getenv("PORT"))
        port = atoi(p_port);

    spdlog::info(
        "Starting HTTP server ({} threads) on {}:{} ...",
        CPPHTTPLIB_THREAD_POOL_COUNT,
        host,
        port);
    server.listen(host.c_str(), port);

    return 0;
}
