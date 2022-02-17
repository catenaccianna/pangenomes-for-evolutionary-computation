# Flags to use regardless of compiler
CFLAGS_all := -Wall -std=c++17

CXX ?= g++
CFLAGS_not_debug := -O3 -DNDEBUG 
CFLAGS_debug := -g

default: test

test:
  $(CXX) $(CFLAGS_all) $(CFLAGS_debug) test.cpp -o test_de_bruijn_graph
  ./test_de_bruijn_graph
