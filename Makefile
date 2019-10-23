FLAGS= -lncurses -lpthread -std=c++11

all: nhex

client: client.cpp oo_client.cpp
	g++ -oclient client.cpp oo_client.cpp $(FLAGS)

server: server.cpp
	g++ -oserver server.cpp $(FLAGS)

debug:
	g++ -onhex nhex_mainloop.cpp oo_nhex.cpp $(FLAGS) -g

nhex: nhex_mainloop.cpp oo_nhex.cpp oo_nhex.hpp
	g++ -onhex nhex_mainloop.cpp oo_nhex.cpp $(FLAGS)

clear:
	rm nhex
