#pragma once

#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>
namespace dbx {
namespace util {
template<typename K>
concept Comparable = requires(K k) {
    { k < k } -> std::convertible_to<bool>;
    { k == k } -> std::convertible_to<bool>;
    { k > k } -> std::convertible_to<bool>;
};

} // namespace util
} // namespace dbx
#endif // UTIL_H