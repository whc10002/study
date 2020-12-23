#include <benchmark/benchmark.h>
#include <set>
#include "have_common_element.h"

void have_common_element_my()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};

        have_common_element_my<std::set<int>>(a, b);
}

void have_common_element_return_value()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};
	have_common_element<std::set<int>::iterator, std::set<int>::iterator>(a.begin(), a.end(), b.begin(), b.end());
}

void have_common_element_hash()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};
	have_common_element_hash<std::set<int>>(a, b);
}

void set_is_intersected()
{
	std::set<int> a{1, 4, 5, 6};
	std::set<int> b{4, 5, 6, 7, 8};
	set_is_intersected(a, b);
}

static void BM_have_common_element_my(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		have_common_element_my();
	}
}

static void BM_set_is_intersected(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		set_is_intersected();
	}
}

static void BM_have_common_element_return_value(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		have_common_element_return_value();
	}
}

static void BM_have_common_element_hash(benchmark::State& state) {
	// Perform setup here
	for (auto _ : state)
	{
		// This code gets timed
		have_common_element_hash();
	}
}

// Register the function as a benchmark
BENCHMARK(BM_have_common_element_my);

// Register the function as a benchmark
BENCHMARK(BM_set_is_intersected);

// Register the function as a benchmark
BENCHMARK(BM_have_common_element_return_value);

// Register the function as a benchmark
BENCHMARK(BM_have_common_element_hash);

BENCHMARK(BM_have_common_element_my)->Iterations(100000);
BENCHMARK(BM_set_is_intersected)->Iterations(100000);
BENCHMARK(BM_have_common_element_return_value)->Iterations(100000);
BENCHMARK(BM_have_common_element_hash)->Iterations(100000);
// Run the benchmark
BENCHMARK_MAIN();

