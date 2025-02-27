#pragma once

#include "status.h"
#include "util/proxy.h"
#include <cstdint>
#include <string_view>
#ifndef LOG_OPS_H
#define LOG_OPS_H
#include <env.h>

namespace dbx {
namespace log {

// log format
enum RecordType {
    ZeroType = 0, // 表示gap
    FullType = 1, // 表示一个Record能够放在一个block中

    FirstType  = 2, // 表示分片Record的开始
    MiddleType = 3, // 表示分片Record的中间部分
    LastType   = 4  // 表示分片Record的结束
};

static constexpr int MaxRecordType = LastType;
static constexpr int BlockSize     = 32 * 1024; // 32KB
static constexpr int HeaderSize    = 4 + 2 + 1; // checksum(4) + length(2) + type(1)

// log writer

class Writer {
  public:
    explicit Writer(pro::proxy<WritableFile> dest);
    Writer(pro::proxy<WritableFile> dest, uint64_t dest_length);

    Writer(const Writer&)            = delete;
    Writer& operator=(const Writer&) = delete;

    Status AddRecord(const std::string_view& slice);

    ~Writer();

  private:
    Status EmitPhysicalRecord(RecordType type, const char* ptr, size_t length); // 写入一个Record

    WritableFile* dest_;
    int           block_offset_;

    uint32_t type_crc_[MaxRecordType + 1];
};

// log reader

class Reader {
  public:
    // clang-format off
    PRO_DEF_MEM_DISPATCH(MemCorruption, Corruption);
    struct Reporter : pro::facade_builder
        :: add_convention<MemCorruption, void(size_t, Status&)>
        :: build{};
    // clang-format on
    Reader(pro::proxy<SequentialFile> file,
           pro::proxy<Reporter>       reporter,
           bool                       checksum,
           uint64_t                   initial_offset);
    Reader(const Reader&)            = delete;
    Reader& operator=(const Reader&) = delete;
    ~Reader();
    bool     ReadRecord(std::string_view record, std::string* scratch);
    uint64_t LastRecordOffset() const;

  private:
    enum {
        kEof       = MaxRecordType + 1,
        kBadRecord = MaxRecordType + 2,
    };
};

} // namespace log
} // namespace dbx

#endif