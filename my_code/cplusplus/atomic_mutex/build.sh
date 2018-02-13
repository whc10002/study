#!/bin/sh
g++ -std=c++11 -O3 -g -o atomic_map -lpthread -latomic  atomic_map.cpp
g++ -std=c++11 -O3 -g -o atomic_mutex -lpthread  atomic_mutex.cpp
