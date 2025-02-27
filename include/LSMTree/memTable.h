#pragma once

#ifndef __MEM_TABLE_H__
#define __MEM_TABLE_H__

#include <LSMTree/skipList.h>

namespace dbx {
class InternalKeyComparator;
class MemTableIterator;

class MemTable {
  public:
    explicit MemTable(const InternalKeyComparator& cmp);

    MemTable(const MemTable&)            = delete; // 禁止复制构造
    MemTable& operator=(const MemTable&) = delete; // 禁止赋值构造

    void Ref() noexcept;
    void Unref() noexcept;

    size_t ApproximateMemoryUsage() const noexcept; // 估计内存使用量

  private:
    ~MemTable(); // 只能自己释放
};
} // namespace dbx

#endif // __MEM_TABLE_H__