# Project-specific settings
PROJECT := memic_model
EMP_DIR := ../Empirical/source

# Flags to use regardless of compiler
CFLAGS_all := -Wall -Wno-unused-function -std=c++17

# Native compiler information
CXX := g++
CFLAGS_nondebug := -O3 -DNDEBUG $(CFLAGS_all)
CFLAGS_debug := -g $(CFLAGS_all)

test:
	$(CXX) $(CFLAGS_debug) test.cpp