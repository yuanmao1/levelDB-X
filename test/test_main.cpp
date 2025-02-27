#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

int main(int argc, char** argv) {
    // 初始化 Google Test
    ::testing::InitGoogleTest(&argc, argv);
    int gtest_result = RUN_ALL_TESTS();

    // 如果 Google Test 测试失败，直接返回
    if (gtest_result != 0) {
        return gtest_result;
    }

    // 运行 Google Benchmark
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::ConsoleReporter reporter;
    ::benchmark::RunSpecifiedBenchmarks(&reporter); // 添加reporter参数

    return 0;
}