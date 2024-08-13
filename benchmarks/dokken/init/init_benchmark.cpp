#include <benchmark/benchmark.h>
#include <CPPVersus.hpp>

static void BM_DOKKEN_INIT(benchmark::State &state) {
    for (auto _ : state) {
        CPPVersus::DokkenAPI(API_KEY);   
    }
}
// Register the function as a benchmark
BENCHMARK(BM_DOKKEN_INIT);
