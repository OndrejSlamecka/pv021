CXXFLAGS=-std=c++14 -pedantic -Wall -Wextra -O3 $(EXTRA_CXXFLAGS)

FAKEVAR:=$(shell mkdir -p bin)

nn: src/main.cpp
	$(CXX) -o bin/$@ $(CXXFLAGS) $^

test_parser: src/test_parser.cpp
	$(CXX) -o bin/$@ $(CXXFLAGS) $^
