#include <LSMTree/skipList.h>
#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>
#include <print>

void n_test() {
    dbx::SkipList<int, int> list;
    for (int i = 5; i > 0; i--) {
        list.insert(rand() % 100, i);
        std::println("insert ----------");
        list.printAll();
    }
    for (int i = 0; i < 10; i++) {
        std::cout << list.find(i) << " ";
        // list.printAll();
    }
};

int main(int argc, char** argv) {
    n_test();
    //::testing::InitGoogleTest(&argc, argv);
    // return RUN_ALL_TESTS();
}
