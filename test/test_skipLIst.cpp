#include <LSMTree/skipList.h>
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
}
//
// TEST(test_skipList, timeCost) {
//     std::println("test timecost begin");
//     dbx::SkipList<int, int> list;
//     auto                    start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < 10000000; i++) {
//         list.insert(i, i);
//     }
//     auto                          end  = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> diff = end - start;
//     std::cout << "insert 10000000 elements cost: " << diff.count() << " ns" << std::endl;
//     auto receive = [](int val) {};
//     start        = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < 10000000; i++) {
//         receive(list.find(i));
//     }
//     end  = std::chrono::high_resolution_clock::now();
//     diff = end - start;
//     std::cout << "find 10000000 elements cost: " << diff.count() << " ns" << std::endl;
//     start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < 10000000; i++) {
//         list.remove(i);
//     }
//     end  = std::chrono::high_resolution_clock::now();
//     diff = end - start;
//     std::cout << "remove 10000000 elements cost: " << diff.count() << " ns" << std::endl;
// }

// 迭代器测试
TEST(test_skipList, iterator_empty) {
    dbx::SkipList<int, int> list;
    auto                    begin = list.begin();
    auto                    end   = list.end();
    EXPECT_EQ(begin, end);
}

TEST(test_skipList, iterator_insert) {
    dbx::SkipList<int, int> list;
    for (int i = 0; i < 10; i++) {
        list.insert(i, i);
    }
    int expected_key = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(it->key, expected_key);
        EXPECT_EQ(it->value, expected_key);
        expected_key++;
    }
    EXPECT_EQ(expected_key, 10);
}

TEST(test_skipList, iterator_remove) {
    dbx::SkipList<int, int> list;
    for (int i = 0; i < 10; i++) {
        list.insert(i, i);
    }
    // 删除偶数键
    for (int i = 0; i < 10; i += 2) {
        list.remove(i);
    }
    std::vector<int> remaining_keys{1, 3, 5, 7, 9};
    size_t           index = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        ASSERT_LT(index, remaining_keys.size());
        EXPECT_EQ(it->key, remaining_keys[index]);
        EXPECT_EQ(it->value, remaining_keys[index]);
        index++;
    }
    EXPECT_EQ(index, remaining_keys.size());
}

TEST(test_skipList, iterator_range_for) {
    dbx::SkipList<int, int> list;
    for (int i = 0; i < 5; i++) {
        list.insert(i, i * 10);
    }
    int count = 0;
    for (const auto& pair : list) {
        EXPECT_EQ(pair.first, count);
        EXPECT_EQ(pair.second, count * 10);
        count++;
    }
    EXPECT_EQ(count, 5);
}

TEST(test_skipList, iterator_increment_operators) {
    dbx::SkipList<int, int> list;
    list.insert(1, 10);
    list.insert(2, 20);
    auto it = list.begin();

    // 测试后置++
    EXPECT_EQ(it->key, 1);
    auto old_it = it++;
    EXPECT_EQ(old_it->key, 1);
    EXPECT_EQ(it->key, 2);

    // 测试前置++
    ++it;
    EXPECT_EQ(it, list.end());
}
