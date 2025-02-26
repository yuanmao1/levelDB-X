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