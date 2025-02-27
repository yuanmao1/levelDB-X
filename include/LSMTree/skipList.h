#pragma once

#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <algorithm>
#include <cstdlib> // 添加此头文件，用于生成随机数
#include <print>
#include <util/concepts.h>

namespace dbx {

template<util::Comparable K, typename V>
struct SkipListNode {
    SkipListNode(K k, V v, int l);
    ~SkipListNode();
    // 设置下一个节点
    void setNxt(int l, SkipListNode* n);

    const int      level;
    const K        key;
    V              value;
    SkipListNode** forward;
};

template<util::Comparable K, typename V>
class SkipList {
  public:
    SkipList();
    ~SkipList();
    void insert(K key, V value) noexcept;
    void remove(K key) noexcept;

    [[nodiscard]]
    V find(K key) const noexcept;
    struct Iterator {}; // TODO: Implement iterator

    [[nodiscard]]
    auto begin() const noexcept;
    [[nodiscard]]
    auto end() const noexcept;

    void printAll() const noexcept;

  private:
    int                 length;
    int                 maxLevel;
    int                 randomLevel();
    SkipListNode<K, V>* head;
    int                 compare(SkipListNode<K, V>* node, K k) const;
};
} // namespace dbx

namespace util = dbx::util;

template<dbx::util::Comparable K, typename V>
dbx::SkipList<K, V>::SkipList() {
    length   = 0;
    maxLevel = 4;
    // 头节点为哨兵节点
    head = new SkipListNode<K, V>(K(), V(), maxLevel);
}

template<util::Comparable K, typename V>
dbx::SkipList<K, V>::~SkipList() {
    // 删除所有节点
    while (head != NULL) {
        SkipListNode<K, V>* temp = head;
        head                     = head->forward[0];
        delete temp;
    }
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::printAll() const noexcept {
    std::println("length: {} maxLevel: {}", length, maxLevel);
    SkipListNode<K, V>* curr;
    for (int i = maxLevel; i >= 0; --i) {
        std::println("iiiiiiiiiiiiiiiiiii{}", i);
        curr = head;
        while (curr != NULL) {
            std::println("level: {} key: {} value: {}", curr->level, curr->key, curr->value);
            curr = curr->forward[i];
        }
        std::println();
    }
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::insert(K key, V value) noexcept {
    std::println("insert key: {} value: {} called", key, value);
    SkipListNode<K, V>* update[maxLevel + 1];
    SkipListNode<K, V>* curr = head;
    for (int i = maxLevel; i >= 0; --i) {
        while (curr->forward[i] != NULL && compare(curr->forward[i], key) < 0) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }
    curr = curr->forward[0];
    if (curr != NULL && curr->key == key) {
        curr->value = value;
        return;
    }
    const int level = randomLevel();
    curr            = new SkipListNode<K, V>(key, value, level);
    for (int i = 0; i <= level; ++i) {
        curr->setNxt(i, update[i]->forward[i]);
        update[i]->setNxt(i, curr);
    }
    ++length;
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::remove(K key) noexcept {
}

template<util::Comparable K, typename V>
int dbx::SkipList<K, V>::randomLevel() {
    int lv = 1;
    while ((std::rand() & 0xFFFF) < (0xFFFF >> 2))
        ++lv;
    return std::min(this->maxLevel, lv);
}

// 哨兵永远是最小的
template<util::Comparable K, typename V>
int dbx::SkipList<K, V>::compare(SkipListNode<K, V>* node, K k) const {
    if (node == head)
        return -1;
    return node->key < k ? -1 : (node->key > k ? 1 : 0);
}

template<util::Comparable K, typename V>
auto dbx::SkipList<K, V>::find(K key) const noexcept -> V {
    return key;
}

// skip list node

template<util::Comparable K, typename V>
dbx::SkipListNode<K, V>::SkipListNode(K k, V v, int l) : key(k), value(v), level(l) {
    this->forward = new SkipListNode*[l + 1];
    for (int i = 0; i <= l; ++i) {
        forward[i] = NULL;
    }
}

template<util::Comparable K, typename V>
dbx::SkipListNode<K, V>::~SkipListNode() {
    if (forward != NULL) {
        delete[] forward;
    }
}

template<util::Comparable K, typename V>
void dbx::SkipListNode<K, V>::setNxt(int l, SkipListNode* n) {
    if (forward[l] == NULL || l > level) {
        return;
    }
    forward[l] = n;
}

#endif // __SKIP_LIST_H__ 为什么有问题