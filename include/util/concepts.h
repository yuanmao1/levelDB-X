#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>

template<typename K>
concept Comparable = requires(K k) {
    { k < k } -> std::convertible_to<bool>;
    { k == k } -> std::convertible_to<bool>;
    { k > k } -> std::convertible_to<bool>;
};

#endif // UTIL_H