#include <LSMTree/skipList.h>
#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

TEST(test_skipList, insert) {
    dbx::SkipList<int, int> list;
    for (int i = 0; i < 10; i++) {
        list.insert(i, i);
    }
}

TEST(test_skipList, remove) {
    dbx::SkipList<int, int> list;
    for (int i = 0; i < 10; i++) {
        list.insert(i, i);
    }
    for (int i = 0; i < 10; i++) {
        list.remove(i);
    }
}

TEST(test_skipList, find) {
    dbx::SkipList<int, int> list;
    for (int i = 0; i < 10; i++) {
        list.insert(i, i);
    }
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(list.find(i), i);
    }
    list.remove(5);
    list.remove(3);
    for (int i = 0; i < 10; i++) {
        if (i != 5 && i != 3) {
            EXPECT_EQ(list.find(i), i);
        }
    }
}

static void BM_insertAndFind(benchmark::State& state) {
    dbx::SkipList<int, int> list;
    auto                    receive = [](int) -> void {};
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            list.insert(i, i);
        }
        for (int i = 0; i < state.range(0); i++) {
            receive(list.find(i));
        }
    }
}

static void BM_insertAndRemove(benchmark::State& state) {
    dbx::SkipList<int, int> list;
    auto                    receive = [](int) -> void {};
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); i++) {
            list.insert(i, i);
        }
        for (int i = 0; i < state.range(0); i++) {
            list.remove(i);
        }
    }
}

BENCHMARK(BM_insertAndFind)->Range(8, 8 << 10);
BENCHMARK(BM_insertAndRemove)->Range(8, 8 << 10);