#include <cstdint>
#include <exception>
#include <format>
#include <iostream>
#include <status.h>
#include <string>
#include <string_view>
#include <util/proxy.h>
#include <vector>

namespace dbx {
// clang-format off
// WritableFile
PRO_DEF_MEM_DISPATCH(MemAppend, Append);
PRO_DEF_MEM_DISPATCH(MemClose, Close);
PRO_DEF_MEM_DISPATCH(MemFlush, Flush);
PRO_DEF_MEM_DISPATCH(MemSync, Sync);
struct WritableFile : pro::facade_builder
    :: add_convention<MemAppend, Status(const std::string_view&)>
    :: add_convention<MemClose, Status()>
    :: add_convention<MemFlush, Status()>
    :: add_convention<MemSync, Status()>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{};

// SequentialFile
PRO_DEF_MEM_DISPATCH(MemRead, Read);
PRO_DEF_MEM_DISPATCH(MemSkip, Skip);
struct SequentialFile : pro::facade_builder
    :: add_convention<MemRead, Status(size_t n, std::string_view*, char*)>
    :: add_convention<MemSkip, Status(uint64_t)>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{};
// RandomAccessFile
struct RandomAccessFile : pro::facade_builder
    :: add_convention<MemRead, Status(uint64_t, size_t, std::string_view*, char*)>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{};
// FileLock
struct FileLock : pro::facade_builder
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{};
// Logger
PRO_DEF_MEM_DISPATCH(MemLogv, Logv);
struct Logger : pro::facade_builder
    :: add_convention<MemLogv, void(const char*)>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{};


// Env
/*
    Env类定义与操作系统交互的接口，提供以下功能：
    文件操作：创建、删除、重命名、获取文件大小：
    目录操作：创建、删除目录、获取目录中的文件列表
    文件锁定：用于多进程并发访问时的文件锁定
    线程和任务调度：启动新线程、调度后台
    日志记录：创建日志文件、记录信息
    时间相关操作：获取当前时间、睡眠
*/
PRO_DEF_MEM_DISPATCH(MemDefault, Default);
// 文件操作
PRO_DEF_MEM_DISPATCH(MemNewSequentialFile, NewSequentialFile);
PRO_DEF_MEM_DISPATCH(MemNewRandomAccessFile, NewRandomAccessFile);
PRO_DEF_MEM_DISPATCH(MemNewWritableFile, NewWritableFile);
PRO_DEF_MEM_DISPATCH(NewAppendableFile, AppendableFile);
PRO_DEF_MEM_DISPATCH(MemFileExists, FileExists);
PRO_DEF_MEM_DISPATCH(MemRemoveFile, RemoveFile);
PRO_DEF_MEM_DISPATCH(MemDeleteFile, DeleteFile);
PRO_DEF_MEM_DISPATCH(MemGetFileSize, GetFileSize);
PRO_DEF_MEM_DISPATCH(MemRenameFile, RenameFile);
// 目录操作
PRO_DEF_MEM_DISPATCH(MemGetChildren, GetChildren);
PRO_DEF_MEM_DISPATCH(MemCreateDir, CreateDir);
PRO_DEF_MEM_DISPATCH(MemRemoveDir, RemoveDir);
PRO_DEF_MEM_DISPATCH(MemDeleteDir, DeleteDir);
PRO_DEF_MEM_DISPATCH(MemGetTestDirectory, GetTestDirectory);
// 文件锁定
PRO_DEF_MEM_DISPATCH(MemLockFile, LockFile);
PRO_DEF_MEM_DISPATCH(MemUnlockFile, UnlockFile);
// 线程和任务调度
PRO_DEF_MEM_DISPATCH(MemSchedule, Schedule);
PRO_DEF_MEM_DISPATCH(MemStartThread, StartThread);
// 日志
PRO_DEF_MEM_DISPATCH(MemNewLogger, NewLogger);
// 时间相关操作
PRO_DEF_MEM_DISPATCH(MemNowMicros, NowMicors);
PRO_DEF_MEM_DISPATCH(MemSleepForMicroseconds, SleepForMicroseconds);

struct Env : pro::facade_builder
    :: add_convention<MemDefault, Env*()>
    :: add_convention<MemNewSequentialFile, Status(const std::string&, pro::proxy<SequentialFile>*)>
    :: add_convention<MemNewRandomAccessFile, Status(const std::string&, pro::proxy<RandomAccessFile>*)>
    :: add_convention<MemNewWritableFile, Status(const std::string&, pro::proxy<WritableFile>*)>
    :: add_convention<NewAppendableFile, Status(const std::string&, pro::proxy<WritableFile>*)>
    :: add_convention<MemFileExists, bool(const std::string&)>
    :: add_convention<MemGetChildren, Status(const std::string&, std::vector<std::string>*)>
    :: add_convention<MemRemoveFile, Status(const std::string&)>
    :: add_convention<MemDeleteFile, Status(const std::string&)>
    :: add_convention<MemCreateDir, Status(const std::string&)>
    :: add_convention<MemRemoveDir, Status(const std::string&)>
    :: add_convention<MemDeleteDir, Status(const std::string&)>
    :: add_convention<MemGetFileSize, Status(const std::string&, uint64_t*)>
    :: add_convention<MemRenameFile, Status(const std::string&, const std::string&)>
    :: add_convention<MemLockFile, Status(const std::string&, pro::proxy<FileLock>*)>
    :: add_convention<MemUnlockFile, Status(pro::proxy<FileLock>)>
    :: add_convention<MemSchedule, void(void (*)(void*), void*)>
    :: add_convention<MemStartThread, void(void (*)(void*), void*)>
    :: add_convention<MemGetTestDirectory, Status(std::string*)>
    :: add_convention<MemNewLogger, Status(const std::string&, pro::proxy<Logger>*)>
    :: add_convention<MemNowMicros, uint64_t()>
    :: add_convention<MemSleepForMicroseconds, void(int)>
    :: support_copy<pro::constraint_level::none>
    :: support_relocation<pro::constraint_level::none>
    :: build{};
// clang-format on
template<typename... Args>
void Log(pro::proxy<Logger> info_log, std::format_string<Args...> fmt, Args... args) {
    if (info_log) {
        try {
            info_log->Logv(std::format(fmt, args...));
        } catch (const std::exception& e) {
            info_log->Logv(e.what());
        }
    } else {
        std::cerr << "info_log is nullptr" << std::endl;
    }
};
Status WriteStringToFile(pro::proxy<Env>         env,
                         const std::string_view& data,
                         const std::string&      fname);
Status ReadFileToString(pro::proxy<Env> env, const std::string& fname, std::string* data);

} // namespace dbx