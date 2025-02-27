#include <cstdint>
#include <expected>
#include <system_error>
#include <util/coding.h>

namespace util = dbx::util;

// 错误码映射实现
std::error_code util::make_error_code(VarIntError e) noexcept {
    static const struct : std::error_category {
        const char* name() const noexcept override {
            return "VarInt";
        }
        std::string message(int ev) const override {
            switch (static_cast<VarIntError>(ev)) {
            case VarIntError::kSuccess:
                return "Success";
            case VarIntError::kInsufficientBuffer:
                return "Insufficient buffer space";
            case VarIntError::kInvalidEncoding:
                return "Invalid VarInt encoding";
            default:
                return "Unknown error";
            }
        }
    } category;
    return {static_cast<int>(e), category};
}

// VarInt32 编码
std::expected<size_t, util::VarIntError> util::encodeVarInt32(std::span<uint8_t> dst,
                                                              uint32_t           value) noexcept {
    size_t bytes_written = 0;
    do {
        if (bytes_written >= dst.size()) [[unlikely]] {
            return std::unexpected(VarIntError::kInsufficientBuffer);
        }
        // 取最低7位，并设置最高位（如果有更多位）
        uint8_t byte = static_cast<uint8_t>(value & 0x7F);
        value >>= 7;
        if (value != 0) {
            byte |= 0x80;
        }
        dst[bytes_written++] = byte;
    } while (value != 0);
    return bytes_written;
}

// VarInt64 编码
std::expected<size_t, util::VarIntError> util::encodeVarInt64(std::span<uint8_t> dst,
                                                              uint64_t           value) noexcept {
    size_t bytes_written = 0;
    do {
        if (bytes_written >= dst.size()) [[unlikely]] {
            return std::unexpected(VarIntError::kInsufficientBuffer);
        }
        // 取最低7位，并设置最高位（如果有更多位）
        uint8_t byte = static_cast<uint8_t>(value & 0x7F);
        value >>= 7;
        if (value != 0) {
            byte |= 0x80;
        }
        dst[bytes_written++] = byte;
    } while (value != 0);
    return bytes_written;
}

// VarInt32 解码
std::expected<std::pair<uint32_t, size_t>, util::VarIntError>
util::decodeVarInt32(std::span<const uint8_t> src) noexcept {
    uint32_t result     = 0;
    size_t   shift      = 0;
    size_t   bytes_read = 0;

    while (true) {
        if (bytes_read >= src.size()) {
            return std::unexpected(VarIntError::kInvalidEncoding);
        }

        uint8_t byte = src[bytes_read++];
        result |= static_cast<uint32_t>(byte & 0x7F) << shift;
        shift += 7;

        // 检查是否继续读取
        if ((byte & 0x80) == 0) {
            break; // 最高位为 0，结束解码
        }

        // 防止移位溢出（VarInt 最多 10 字节表示 64 位整数）
        if (shift >= 32 && (byte & 0x80) != 0) {
            return std::unexpected(VarIntError::kInvalidEncoding);
        }
    }

    return std::make_pair(result, bytes_read);
}

// VarInt64 解码
std::expected<std::pair<uint64_t, size_t>, util::VarIntError>
util::decodeVarInt64(std::span<const uint8_t> src) noexcept {
    uint64_t result     = 0;
    size_t   shift      = 0;
    size_t   bytes_read = 0;

    while (true) {
        if (bytes_read >= src.size()) {
            return std::unexpected(VarIntError::kInvalidEncoding);
        }

        uint8_t byte = src[bytes_read++];
        result |= static_cast<uint64_t>(byte & 0x7F) << shift;
        shift += 7;

        // 检查是否继续读取
        if ((byte & 0x80) == 0) {
            break; // 最高位为 0，结束解码
        }

        // 防止移位溢出（VarInt 最多 10 字节表示 64 位整数）
        if (shift >= 64 && (byte & 0x80) != 0) {
            return std::unexpected(VarIntError::kInvalidEncoding);
        }
    }

    return std::make_pair(result, bytes_read);
}