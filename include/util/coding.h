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

// 如果编码成功，返回编码后的字节数；否则返回错误码
std::expected<size_t, VarIntError> encodeVarInt32(std::span<uint8_t> dst, uint32_t value) noexcept;
std::expected<size_t, VarIntError> encodeVarInt64(std::span<uint8_t> dst, uint64_t value) noexcept;

std::expected<std::pair<uint32_t, size_t>, VarIntError>
decodeVarInt32(std::span<const uint8_t> src) noexcept;

std::expected<std::pair<uint64_t, size_t>, VarIntError>
decodeVarInt64(std::span<const uint8_t> src) noexcept;

} // namespace util
} // namespace dbx

#endif // CODING_H