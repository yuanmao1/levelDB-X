#include <LSMTree/skipList.h>
#include <chrono>
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

TEST(test_skipList, timeCost) {
    dbx::SkipList<int, int> list;
    auto                    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000000; i++) {
        list.insert(i, i);
    }
    auto                          end  = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "insert 10000000 elements cost: " << diff.count() << " ns" << std::endl;
    auto receive = [](int val) {};
    start        = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000000; i++) {
        receive(list.find(i));
    }
    end  = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "find 10000000 elements cost: " << diff.count() << " ns" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000000; i++) {
        list.remove(i);
    }
    end  = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "remove 10000000 elements cost: " << diff.count() << " ns" << std::endl;
}