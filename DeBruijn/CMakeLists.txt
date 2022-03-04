cmake_minimum_required(VERSION 3.21)
project(DeBruijn)

set(CMAKE_CXX_STANDARD 17)

add_executable(DeBruijn main.cpp
        DeBruijnGraph.cpp DeBruijnGraph.h
        DeBruijnVertex.cpp DeBruijnVertex.h
        DBGraphValue.cpp DBGraphValue.h)
