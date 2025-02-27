#pragma once

#ifndef LOG_OPS_H
#define LOG_OPS_H

namespace dbx {
namespace log {

// log format
enum RecodeType {
    ZeroType = 0, // 表示gap
    FullType = 1, // 表示一个Recode能够放在一个block中

    FirstType  = 2, // 表示分片Recode的开始
    MiddleType = 3, // 表示分片Recode的中间部分
    LastType   = 4  // 表示分片Recode的结束
};

static constexpr int MaxRecordType = LastType;
static constexpr int BlockSize     = 32 * 1024; // 32KB
static constexpr int HeaderSize    = 4 + 2 + 1; // checksum(4) + length(2) + type(1)

} // namespace log
} // namespace dbx

#endif