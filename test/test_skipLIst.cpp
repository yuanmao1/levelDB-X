#include <LSMTree/skipList.h>
#include <gtest/gtest.h>

TEST(SkipList, insert) {
    db::SkipList<int, int> list;
    list.insert(1, 1);
    list.insert(2, 2);
    list.insert(3, 3);
    list.insert(4, 4);
    list.insert(5, 5);
    list.insert(6, 6);
    list.insert(7, 7);
    list.insert(8, 8);
    list.insert(9, 9);
    list.insert(10, 10);
}

TEST(SkipList, remove) {
    db::SkipList<int, int> list;
    list.insert(1, 1);
    list.insert(2, 2);
    list.insert(3, 3);
    list.insert(4, 4);
    list.insert(5, 5);
    list.insert(6, 6);
    list.insert(7, 7);
    list.insert(8, 8);
    list.insert(9, 9);
    list.insert(10, 10);
    list.remove(1);
    list.remove(2);
    list.remove(3);
    list.remove(4);
    list.remove(5);
    list.remove(6);
    list.remove(7);
    list.remove(8);
    list.remove(9);
    list.remove(10);
}

TEST(SkipList, find) {
    db::SkipList<int, int> list;
    list.insert(1, 1);
    list.insert(2, 2);
    list.insert(3, 3);
    list.insert(4, 4);
    list.insert(5, 5);
    list.insert(6, 6);
    list.insert(7, 7);
    list.insert(8, 8);
    list.insert(9, 9);
    list.insert(10, 10);
    EXPECT_EQ(list.find(1), 1);
    EXPECT_EQ(list.find(2), 2);
    EXPECT_EQ(list.find(3), 3);
    EXPECT_EQ(list.find(4), 4);
    EXPECT_EQ(list.find(5), 5);
    EXPECT_EQ(list.find(6), 6);
    EXPECT_EQ(list.find(7), 7);
    EXPECT_EQ(list.find(8), 8);
    EXPECT_EQ(list.find(9), 9);
    EXPECT_EQ(list.find(10), 10);
}