#!/bin/bash

g++ bench_mark.cpp -std=c++11  -O3 -isystem /usr/local/include/benchmark \
  -lbenchmark -lpthread -o benchmark
