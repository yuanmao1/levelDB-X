#pragma once

#include "util/coding.h"
#ifndef CODING_H
#define CODING_H

#include <cstdint>
#include <expected>
#include <span>
#include <system_error>

namespace dbx {
namespace util {

// VarInt 编码错误
enum class VarIntError {
    kSuccess = 0,
    kInsufficientBuffer, // 缓冲区不足
    kInvalidEncoding,    // 编码无效或数据不足
};

// VarInt 错误处理
std::error_code make_error_code(VarIntError e) noexcept;

std::expected<size_t, VarIntError> encodeVarInt32(std::span<char> dst, uint32_t value) noexcept;
std::expected<size_t, VarIntError> encodeVarInt64(std::span<char> dst, uint64_t value) noexcept;

std::expected<uint32_t, VarIntError> decodeVarInt32(std::span<const char> src) noexcept;
std::expected<uint64_t, VarIntError> decodeVarInt64(std::span<const char> src) noexcept;

} // namespace util
} // namespace dbx

#endif // CODING_H