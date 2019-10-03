FLAGS= -lncurses -lpthread -std=c++11

all: nhex

nhex: nhex_mainloop.cpp oo_nhex.cpp oo_nhex.hpp
	g++ -onhex nhex_mainloop.cpp oo_nhex.cpp $(FLAGS)

clear:
	rm nhex
