#include <env.h>

namespace dbx {

// 写入字符串到文件
static Status DoWriteStringToFile(pro::proxy<Env>&        env,
                                  const std::string_view& data,
                                  const std::string&      fname,
                                  bool                    should_sync) {
    pro::proxy<WritableFile> file;
    Status                   status = env->NewWritableFile(fname, &file);
    if (!status.ok()) {
        return status;
    }
    status = file->Append(data);
    if (status.ok() && should_sync) {
        status = file->Sync();
    }
    if (status.ok()) {
        status = file->Close();
    }
    if (!status.ok()) {
        env->RemoveFile(fname);
    }
    return status;
}

Status WriteStringToFile(pro::proxy<Env>         env,
                         const std::string_view& data,
                         const std::string&      fname) {
    return DoWriteStringToFile(env, data, fname, false);
}

Status WriteStringToFileSync(pro::proxy<Env>         env,
                             const std::string_view& data,
                             const std::string&      fname) {
    return DoWriteStringToFile(env, data, fname, true);
}

// 从文件中读取字符串
Status ReadFileToString(pro::proxy<Env> env, const std::string& fname, std::string* data) {
    data->clear();
    pro::proxy<SequentialFile> file;
    Status                     status = env->NewSequentialFile(fname, &file);
    if (!status.ok()) {
        return status;
    }
    static constexpr size_t kBufferSize = 8192;
    char*                   buffer      = new char[kBufferSize];
    while (true) {
        std::string_view view;
        status = file->Read(kBufferSize, &view, buffer);
        if (!status.ok()) {
            break;
        }
        data->append(view.data(), view.size());
        if (view.size() < kBufferSize) {
            break;
        }
    }
    delete[] buffer;
    return status;
}

} // namespace dbx