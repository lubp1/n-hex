FLAGS= -lncurses -lpthread -std=c++11

all: nhex

cliente: client/client.cpp client/oo_client.cpp
	g++ -ocliente client/client.cpp client/oo_client.cpp $(FLAGS)

debug_cliente:
	g++ -ocliente client/client.cpp client/oo_client.cpp $(FLAGS) -g

debug_servidor:
	g++ -oservidor server/server.cpp server/oo_server.cpp $(FLAGS) -g
	
servidor: server/server.cpp server/oo_server.cpp server/oo_server.hpp
	g++ -oservidor server/server.cpp server/oo_server.cpp $(FLAGS)

clear:
	rm servidor cliente
