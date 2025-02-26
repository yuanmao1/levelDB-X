#pragma once

#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <util/concepts.h>
namespace dbx {

template<util::Comparable K, typename V>
struct SkipListNode {
    int            level;
    K              key;
    V              value;
    SkipListNode** forward;

    SkipListNode(K k, V v, int l, SkipListNode* nxt = NULL);

    ~SkipListNode();
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

  private:
    int                 length;
    int                 maxLevel;
    int                 randomLevel();
    SkipListNode<K, V>* head;
};
} // namespace dbx

namespace util = dbx::util;

template<dbx::util::Comparable K, typename V>
dbx::SkipList<K, V>::SkipList() {
}

template<util::Comparable K, typename V>
dbx::SkipList<K, V>::~SkipList() {
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::insert(K key, V value) noexcept {
}

template<util::Comparable K, typename V>
void dbx::SkipList<K, V>::remove(K key) noexcept {
}

template<util::Comparable K, typename V>
int dbx::SkipList<K, V>::randomLevel() {
    int lv = 1;
    while ((rand() & 0xFFFF) < (0xFFFF >> 2))
        ++lv;
    return min(this->maxLevel, lv);
}

template<util::Comparable K, typename V>
auto dbx::SkipList<K, V>::find(K key) const noexcept -> V {
    return key;
}

template<util::Comparable K, typename V>
dbx::SkipListNode<K, V>::SkipListNode(K k, V v, int l, SkipListNode* nxt) {
    key     = k;
    value   = v;
    level   = l;
    forward = new SkipListNode*[l + 1];
    for (int i = 0; i <= l; ++i)
        forward[i] = nxt;
}

template<util::Comparable K, typename V>
dbx::SkipListNode<K, V>::~SkipListNode() {
    if (forward != NULL)
        delete[] forward;
}

#endif // __SKIP_LIST_H__