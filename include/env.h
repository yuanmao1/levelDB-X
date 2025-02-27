#include <cstdint>
#include <status.h>
#include <string>
#include <string_view>
#include <util/proxy.h>
#include <vector>

namespace dbx {
class FileLock;
class Logger;
// clang-format off
// WritableFile
PRO_DEF_MEM_DISPATCH(MemAppend, Append);
PRO_DEF_MEM_DISPATCH(MemClose, Close);
PRO_DEF_MEM_DISPATCH(MemFlush, Flush);
PRO_DEF_MEM_DISPATCH(MemSync, Sync);
struct WritableFile : pro::facade_builder
    ::add_convention<MemAppend, Status(const std::string_view&)>
    ::add_convention<MemClose, Status()>
    ::add_convention<MemFlush, Status()>
    ::add_convention<MemSync, Status()>
    ::support_copy<pro::constraint_level::none>
    ::support_relocation<pro::constraint_level::none>
    ::build {};

// SequentialFile
PRO_DEF_MEM_DISPATCH(MemRead, Read);
PRO_DEF_MEM_DISPATCH(MemSkip, Skip);
struct SequentailFile : pro::facade_builder
    :: add_convention<MemRead, Status(size_t n, std::string_view*, char*)>
    :: add_convention<MemSkip, Status(uint64_t)>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{}
// RandomAccessFile
struct RandomAccessFile : pro::facade_builder
    :: add_convention<MemRead, Status(uint64_t, size_t, std::string_view*, char*)>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{}
// FileLock
struct FileLock : pro::facade_builder
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{}

// Env
PRO_DEF_MEM_DISPATCH(MemDefault, Default);
PRO_DEF_MEM_DISPATCH(MemNewSequentialFile, NewSequentialFile);
PRO_DEF_MEM_DISPATCH(MemNewRandomAccessFile, NewRandomAccessFile);
PRO_DEF_MEM_DISPATCH(MemNewWritableFile, NewWritableFile);
PRO_DEF_MEM_DISPATCH(NewAppendableFile, AppendableFile);
PRO_DEF_MEM_DISPATCH(MemFileExists, FileExists);
PRO_DEF_MEM_DISPATCH(MemGetChildren, GetChildren);
PRO_DEF_MEM_DISPATCH(MemRemoveFile, RemoveFile);
PRO_DEF_MEM_DISPATCH(MemDeleteFile, DeleteFile);
PRO_DEF_MEM_DISPATCH(MemCreateDir, CreateDir);
PRO_DEF_MEM_DISPATCH(MemRemoveDir, RemoveDir);
PRO_DEF_MEM_DISPATCH(MemDeleteDir, DeleteDir);
PRO_DEF_MEM_DISPATCH(MemGetFileSize, GetFileSize);
PRO_DEF_MEM_DISPATCH(MemRenameFile, RenameFile);
PRO_DEF_MEM_DISPATCH(MemLockFile, LockFile);
PRO_DEF_MEM_DISPATCH(MemUnlockFile, UnlockFile);
PRO_DEF_MEM_DISPATCH(MemSchedule, Schedule);
PRO_DEF_MEM_DISPATCH(MemStartThread, StartThread);
PRO_DEF_MEM_DISPATCH(MemGetTestDirectory, GetTestDirectory);
PRO_DEF_MEM_DISPATCH(MemNewLogger, NewLogger);
PRO_DEF_MEM_DISPATCH(MemNowMicros, NowMicors);
PRO_DEF_MEM_DISPATCH(MemSleepForMicroseconds, SleepForMicroseconds);

struct Env : pro::facade_builder
    ::add_convention<MemDefault, Env*()>
    ::add_convention<MemNewSequentialFile, Status(const std::string&, SequentialFile**)>
    ::add_convention<MemNewRandomAccessFile, Status(const std::string&, RandomAccessFile**)>
    ::add_convention<MemNewWritableFile, Status(const std::string&, WritableFile**)>
    ::add_convention<NewAppendableFile, Status(const std::string&, WritableFile**)>
    ::add_convention<MemFileExists, bool(const std::string&)>
    ::add_convention<MemGetChildren, Status(const std::string&, std::vector<std::string>*)>
    ::add_convention<MemRemoveFile, Status(const std::string&)>
    ::add_convention<MemDeleteFile, Status(const std::string&)>
    ::add_convention<MemCreateDir, Status(const std::string&)>
    ::add_convention<MemRemoveDir, Status(const std::string&)>
    ::add_convention<MemDeleteDir, Status(const std::string&)>
    ::add_convention<MemGetFileSize, Status(const std::string&, uint64_t*)>
    ::add_convention<MemRenameFile, Status(const std::string&, const std::string&)>
    ::add_convention<MemLockFile, Status(const std::string&, FileLock**)>
    ::add_convention<MemUnlockFile, Status(FileLock*)>
    ::add_convention<MemSchedule, void(void (*)(void*), void*)>
    ::add_convention<MemStartThread, void(void (*)(void*), void*)>
    ::add_convention<MemGetTestDirectory, Status(std::string*)>
    ::add_convention<MemNewLogger, Status(const std::string&, Logger**)>
    ::add_convention<MemNowMicros, uint64_t()>
    ::add_convention<MemSleepForMicroseconds, void(int)>
    ::support_copy<pro::constraint_level::none>
    ::support_relocation<pro::constraint_level::none>
    ::build {};
// clang-format on

} // namespace dbx