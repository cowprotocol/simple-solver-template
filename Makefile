main: main.cpp token.cpp order.cpp aux.cpp
	g++ -Wall main.cpp token.cpp order.cpp aux.cpp optimize.cpp -o main -lgmp