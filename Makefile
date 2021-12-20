main_solver: ./src/solver/main_solver.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/api/aux.cpp
	g++ -Wall ./src/solver/main_solver.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/components/amm.cpp ./src/api/aux.cpp ./src/solver/optimize.cpp -o ./src/solver/main_solver -lgmp

main_solver_server: ./src/solver/main_solver_server.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/api/aux.cpp
	g++ -Wall -I ~/spdlog/include/spdlog -I ./external_libs/ ./src/solver/main_solver_server.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/components/amm.cpp ./src/api/aux.cpp ./src/solver/optimize.cpp -o ./src/solver/main_solver_server -lgmp


