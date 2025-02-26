#include "util/concepts.h"
#include <LSMTree/skipList.h>

template<Comparable K, typename V>
db::SkipList<K, V>::SkipList() {
}

template<Comparable K, typename V>
db::SkipList<K, V>::~SkipList() {
}

template<Comparable K, typename V>
void db::SkipList<K, V>::insert(K key, V value) noexcept {
}

template<Comparable K, typename V>
void db::SkipList<K, V>::remove(K key) noexcept {
}

template<Comparable K, typename V>
int db::SkipList<K, V>::randomLevel() {
    int lv = 1;
    while ((rand() & 0xFFFF) < (0xFFFF >> 2))
        ++lv;
    return min(this->maxLevel, lv);
}

template<Comparable K, typename V>
auto db::SkipList<K, V>::find(K key) const noexcept -> V {
    return V();
}

template<Comparable K, typename V>
db::SkipListNode<K, V>::SkipListNode(K k, V v, int l, SkipListNode* nxt) {
    key     = k;
    value   = v;
    level   = l;
    forward = new SkipListNode*[l + 1];
    for (int i = 0; i <= l; ++i)
        forward[i] = nxt;
}

template<Comparable K, typename V>
db::SkipListNode<K, V>::~SkipListNode() {
    if (forward != NULL)
        delete[] forward;
}