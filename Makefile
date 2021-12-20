solver: ./src/solver/main_solver.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/api/aux.cpp
	g++ -Wall ./src/solver/main_solver.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/components/amm.cpp ./src/api/aux.cpp ./src/solver/optimize.cpp -o ./src/solver/solver -lgmp

solver_server: ./src/solver/main_solver_server.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/api/aux.cpp
	g++ -Wall -I ~/spdlog/spdlog/include/ -I ./external_libs/ ./src/solver/main_solver_server.cpp ./src/components/token.cpp ./src/components/order.cpp ./src/components/amm.cpp ./src/api/aux.cpp ./src/solver/optimize.cpp -o ./src/solver/solver_server -lgmp


