#!/bin/bash

g++ bench_mark.cpp -std=c++11 -isystem /usr/local/include/benchmark \
  -Lbenchmark/build/src -lbenchmark -lpthread -o benchmark
