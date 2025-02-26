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
        uint8_t byte = value & 0x7F;
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
        uint8_t byte = value & 0x7F;
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
    uint32_t value      = 0;
    size_t   bytes_read = 0;
    for (int shift = 0; shift < 32; shift += 7) {
        if (bytes_read >= src.size()) {
            return std::unexpected(VarIntError::kInvalidEncoding);
        }
        uint8_t byte = src[bytes_read++];
        value |= (byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            return std::make_pair(value, bytes_read);
        }
    }
    return std::unexpected(VarIntError::kInvalidEncoding);
}

// VarInt64 解码
std::expected<std::pair<uint64_t, size_t>, util::VarIntError>
util::decodeVarInt64(std::span<const uint8_t> src) noexcept {
    uint64_t value      = 0;
    size_t   bytes_read = 0;
    for (int shift = 0; shift < 64; shift += 7) {
        if (bytes_read >= src.size()) {
            return std::unexpected(VarIntError::kInvalidEncoding);
        }
        uint8_t byte = src[bytes_read++];
        value |= (byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            return std::make_pair(value, bytes_read);
        }
    }
    return std::unexpected(VarIntError::kInvalidEncoding);
}