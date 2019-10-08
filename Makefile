FLAGS= -lncurses -lpthread -std=c++11

all: nhex

debug:
	g++ -onhex nhex_mainloop.cpp oo_nhex.cpp $(FLAGS) -g

nhex: nhex_mainloop.cpp oo_nhex.cpp oo_nhex.hpp
	g++ -onhex nhex_mainloop.cpp oo_nhex.cpp $(FLAGS)

clear:
	rm nhex
