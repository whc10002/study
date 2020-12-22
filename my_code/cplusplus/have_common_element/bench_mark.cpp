#include <benchmark/benchmark.h>
#include <set>
#include "have_common_element.h"

void WithException()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};

        have_common_element<std::set<int>>(a, b);
}

void WithReturn()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};
	have_common_element<std::set<int>::iterator, std::set<int>::iterator>(a.begin(), a.end(), b.begin(), b.end());
}

void WithException_1()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};
	set_is_intersected(a, b);
}

static void BM_WithException(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		WithException();
	}
}

static void BM_WithException_1(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		WithException_1();
	}
}

static void BM_WithReturn(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		WithReturn();
	}
}
// Register the function as a benchmark
BENCHMARK(BM_WithException);

// Register the function as a benchmark
BENCHMARK(BM_WithException_1);

// Register the function as a benchmark
BENCHMARK(BM_WithReturn);
// Run the benchmark
BENCHMARK_MAIN();

