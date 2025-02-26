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

#endif // __SKIP_LIST_H__