#pragma once

#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <algorithm>
#include <cstdlib>
#include <random>
#include <util/concepts.h>

namespace dbx {

template<util::Comparable K, typename V>
struct SkipListNode {
    SkipListNode(K k, V v, int l);
    ~SkipListNode();
    // 设置下一个节点
    void setNxt(int l, SkipListNode* n);

    const int      level; // 节点最高层
    const K        key;
    V              value;
    SkipListNode** forward; // 记录各层的下一个节点
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
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SkipListNode<K, V>*;
        using reference         = SkipListNode<K, V>*;
        using pointer           = SkipListNode<K, V>**;

        SkipListNode<K, V>* item = nullptr;
        explicit iterator(SkipListNode<K, V>* item) noexcept : item(item) {
        }

        [[nodiscard]]
        auto operator*() const noexcept -> SkipListNode<K, V>* {
            assert(item != nullptr);
            return item;
        }

        [[nodiscard]]
        auto operator->() const noexcept -> SkipListNode<K, V>** {
            assert(item != nullptr);
            return &item;
        }

        auto operator++() noexcept -> iterator& {
        }

        auto operator++(int) noexcept -> iterator {
        }

        auto operator==(const iterator&) const noexcept -> bool = default;
    };

    [[nodiscard]]
    auto begin() const noexcept;
    [[nodiscard]]
    auto end() const noexcept;

  private:
    uint32_t                 length;
    constexpr static uint8_t maxLevel = 32;
    uint8_t                  randomLevel() const;
    SkipListNode<K, V>*      head;
    int                      compare(SkipListNode<K, V>* node, K k) const;
};
} // namespace dbx

namespace util = dbx::util;

template<dbx::util::Comparable K, typename V>
dbx::SkipList<K, V>::SkipList() {
    length = 0;
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
void dbx::SkipList<K, V>::insert(K key, V value) noexcept {
    // std::println("insert key: {} value: {} called", key, value);
    SkipListNode<K, V>** update = new SkipListNode<K, V>*[maxLevel + 1];
    SkipListNode<K, V>*  curr   = head;
    for (int i = maxLevel; i >= 0; --i) {
        while (curr->forward[i] != NULL && compare(curr->forward[i], key) < 0) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }
    curr = curr->forward[0];
    if (curr != NULL && curr->key == key) {
        curr->value = value;
        delete[] update;
        return;
    }
    const int level = randomLevel();
    curr            = new SkipListNode<K, V>(key, value, level);
    for (int i = 0; i <= level; ++i) {
        curr->setNxt(i, update[i]->forward[i]);
        update[i]->setNxt(i, curr);
    }
    ++length;
    delete[] update;
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::remove(K key) noexcept {
    SkipListNode<K, V>** update = new SkipListNode<K, V>*[maxLevel + 1];
    SkipListNode<K, V>*  curr   = head;
    for (int i = maxLevel; i >= 0; --i) {
        while (curr->forward[i] != NULL && compare(curr->forward[i], key) < 0) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }
    curr = curr->forward[0];

    if (curr == NULL || curr->key != key) {
        delete[] update;
        return;
    }

    for (int i = 0; i <= curr->level; ++i) {
        if (update[i]->forward[i] != curr) {
            break;
        }
        update[i]->setNxt(i, curr->forward[i]);
    }
    delete curr;
    --length;

    delete[] update;
}

template<util::Comparable K, typename V>
auto dbx::SkipList<K, V>::find(K key) const noexcept -> V {
    // SkipListNode<K, V>** update = new SkipListNode<K, V>*[maxLevel + 1];
    SkipListNode<K, V>* curr = head;
    for (int i = maxLevel; i >= 0; --i) {
        while (curr->forward[i] != NULL && compare(curr->forward[i], key) < 0) {
            curr = curr->forward[i];
        }
        // update[i] = curr;
    }
    curr = curr->forward[0];
    if (curr != NULL && curr->key == key) {
        // delete[] update;
        return curr->value;
    }

    // delete[] update;
    return V();
}

template<util::Comparable K, typename V>
uint8_t dbx::SkipList<K, V>::randomLevel() const {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 0xFFFF);

    int lv = 1;
    while (dis(gen) < (0xFFFF >> 2) && lv < maxLevel) {
        ++lv;
    }
    return std::min(maxLevel, lv);
}

// 哨兵永远是最小的
template<util::Comparable K, typename V>
int dbx::SkipList<K, V>::compare(SkipListNode<K, V>* node, K k) const {
    if (node == head)
        return -1;
    return node->key < k ? -1 : (node->key > k ? 1 : 0);
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
    if (forward == NULL || l > level) { // 确保不越界
        return;
    }
    forward[l] = n;
}

#endif // __SKIP_LIST_H__