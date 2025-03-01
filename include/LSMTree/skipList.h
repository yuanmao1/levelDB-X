#pragma once

#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <cstdlib>
#include <mimalloc.h>
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

    SkipList(const SkipList&)            = delete;
    SkipList& operator=(const SkipList&) = delete;

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
        explicit iterator(SkipListNode<K, V>* item) noexcept : item(item) {}

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
            if (item) {
                item = item->forward[0];
            }
            return *this;
        }

        auto operator++(int) noexcept -> iterator {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        auto operator==(const iterator&) const noexcept -> bool = default;
    };

    [[nodiscard]]
    auto begin() const noexcept;
    [[nodiscard]]
    auto end() const noexcept;

  private:
    uint32_t length;
    uint8_t  randomLevel() const;
    int      compare(SkipListNode<K, V>* node, K k) const;

    constexpr static uint8_t maxLevel = 32;
    SkipListNode<K, V>*      head;
};
} // namespace dbx

namespace util = dbx::util;

template<util::Comparable K, typename V>
auto dbx::SkipList<K, V>::begin() const noexcept {
    return iterator(head->forward[0]);
}

template<util::Comparable K, typename V>

auto dbx::SkipList<K, V>::end() const noexcept {
    return iterator(nullptr);
}

template<util::Comparable K, typename V>
dbx::SkipList<K, V>::SkipList() {
    length = 0;
    // 头节点为哨兵节点
    head = static_cast<SkipListNode<K, V>*>(mi_malloc(sizeof(SkipListNode<K, V>)));
    if (head == nullptr) {
        throw std::bad_alloc();
    }
    new (head) SkipListNode<K, V>(K(), V(), maxLevel);
}

template<util::Comparable K, typename V>
dbx::SkipList<K, V>::~SkipList() {
    // 删除所有节点
    while (head != nullptr) {
        SkipListNode<K, V>* temp = head;
        head                     = head->forward[0];
        temp->~SkipListNode(); // 显式调用析构函数
        mi_free(temp);
    }
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::insert(K key, V value) noexcept {
    SkipListNode<K, V>** update =
        static_cast<SkipListNode<K, V>**>(mi_malloc((maxLevel + 1) * sizeof(SkipListNode<K, V>*)));
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
        mi_free(update);
        return;
    }
    const int level = randomLevel();
    curr            = static_cast<SkipListNode<K, V>*>(mi_malloc(sizeof(SkipListNode<K, V>)));
    if (curr == nullptr) {
        mi_free(update);
        return;
    }
    new (curr) SkipListNode<K, V>(key, value, level);
    for (int i = 0; i <= level; ++i) {
        curr->setNxt(i, update[i]->forward[i]);
        update[i]->setNxt(i, curr);
    }
    ++length;
    mi_free(update);
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::remove(K key) noexcept {
    SkipListNode<K, V>** update =
        static_cast<SkipListNode<K, V>**>(mi_malloc((maxLevel + 1) * sizeof(SkipListNode<K, V>*)));
    SkipListNode<K, V>* curr = head;
    for (int i = maxLevel; i >= 0; --i) {
        while (curr->forward[i] != NULL && compare(curr->forward[i], key) < 0) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }
    curr = curr->forward[0];

    if (curr == NULL || curr->key != key) {
        mi_free(update);
        return;
    }

    for (int i = 0; i <= curr->level; ++i) {
        if (update[i]->forward[i] != curr) {
            break;
        }
        update[i]->setNxt(i, curr->forward[i]);
    }
    curr->~SkipListNode();
    mi_free(curr);
    --length;

    mi_free(update);
}

template<util::Comparable K, typename V>
auto dbx::SkipList<K, V>::find(K key) const noexcept -> V {
    SkipListNode<K, V>* curr = head;
    for (int i = maxLevel; i >= 0; --i) {
        while (curr->forward[i] != NULL && compare(curr->forward[i], key) < 0) {
            curr = curr->forward[i];
        }
    }
    curr = curr->forward[0];
    if (curr != NULL && curr->key == key) {
        return curr->value;
    }

    return V();
}

template<util::Comparable K, typename V>
uint8_t dbx::SkipList<K, V>::randomLevel() const {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 0xFFFF);

    uint8_t lv = 1;
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

template<util::Comparable K, typename V>
dbx::SkipListNode<K, V>::SkipListNode(K k, V v, int l) : key(k), value(v), level(l) {
    this->forward = static_cast<SkipListNode**>(mi_malloc((l + 1) * sizeof(SkipListNode*)));
    if (this->forward == nullptr) {
        throw std::bad_alloc();
    }
    for (int i = 0; i <= l; ++i) {
        forward[i] = NULL;
    }
}

template<util::Comparable K, typename V>
dbx::SkipListNode<K, V>::~SkipListNode() {
    if (forward != NULL) {
        mi_free(forward);
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