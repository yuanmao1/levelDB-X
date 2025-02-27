#include <assert.h>
#include <status.h>

dbx::Status::Status(dbx::Status::Code       code,
                    const std::string_view& msg,
                    const std::string_view& msg2 = std::string_view()) { // msg2默认为空
    assert(code != kOk);
    const size_t len1   = msg.size();
    const size_t len2   = msg2.size();
    const size_t size   = len1 + (len2 ? (2 + len2) : 0);
    char*        result = new char[size + 5]; // 这里的5是header
    memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(code);
    memcpy(result + 5, msg.data(), len1);
    if (len2) {
        result[5 + len1] = ':';
        result[6 + len1] = ' ';
        memcpy(result + 7 + len1, msg2.data(), len2);
    }
    state_ = result;
}

std::string dbx::Status::ToString() const {
    if (state_ == nullptr) {
        return "OK";
    } else {
        char        tmp[30];
        const char* type;
        switch (code()) {
        case kOk:
            type = "OK";
            break;
        case kNotFound:
            type = "NotFound: ";
            break;
        case kCorruption:
            type = "Corruption: ";
            break;
        case kNotSupported:
            type = "Not implemented: ";
            break;
        case kInvalidArgument:
            type = "Invalid argument: ";
            break;
        case kIOError:
            type = "IO error: ";
            break;
        default:
            std::snprintf(tmp, sizeof(tmp), "Unknown code(%d): ", static_cast<int>(code()));
            type = tmp;
            break;
        }
        std::string result(type);
        uint32_t    length;
        std::memcpy(&length, state_, sizeof(length));
        result.append(state_ + 5, length);
        return result;
    }
}

const char* dbx::Status::CopyState(const char* state) {
    uint32_t size;
    std::memcpy(&size, state, sizeof(size));
    char* result = new char[size + 5];
    std::memcpy(result, state, size + 5);
    return result;
}