#ifndef FILE_LOAD_JOB_H_
#define FILE_LOAD_JOB_H_

// library includes
#include <functional>

// engine includes
#include "Data\PooledString.h"
#include "Jobs\InterfaceJob.h"
#include "Util\FileUtils.h"

namespace engine {
namespace jobs {

class FileLoadJob : public InterfaceJob
{
public:
    FileLoadJob(const engine::data::PooledString& i_file_name, const std::function<void(const engine::util::FileUtils::FileData)>& i_callback, bool i_does_cache_file = true);
    ~FileLoadJob();

    // implement InterfaceJob
    void DoWork() override;

    // accessors and mutators
    inline const engine::data::PooledString& GetFileName() const;
    inline void SetFileName(const engine::data::PooledString& i_file_name);

    inline const std::function<void(const engine::util::FileUtils::FileData)>& GetCallback() const;
    inline void SetCallback(const std::function<void(const engine::util::FileUtils::FileData)>& i_callback);

    inline bool GetDoesCacheFile() const;
    inline void SetDoesCacheFile(bool i_does_cache_file);

private:
    FileLoadJob(const FileLoadJob&) = delete;
    FileLoadJob(FileLoadJob&&) = delete;

    FileLoadJob& operator=(const FileLoadJob&) = delete;
    FileLoadJob& operator=(FileLoadJob&&) = delete;

    std::function<void(const engine::util::FileUtils::FileData)>                callback_;
    engine::data::PooledString                                                  file_name_;
    bool                                                                        does_cache_file_;
};

} // namespace jobs
} // namespace engine

#include "FileLoadJob-inl.h"

#endif // FILE_LOAD_JOB_H_
