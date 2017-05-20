#include "FileUtils.h"

namespace engine {
namespace util {

inline FileUtils* FileUtils::Get()
{
    return FileUtils::instance_;
}

inline const FileUtils::FileData FileUtils::GetFileFromCache(const engine::data::PooledString& i_file_name) const
{
    return GetFileFromCache(engine::data::HashedString::Hash(i_file_name));
}

inline const FileUtils::FileData FileUtils::GetFileFromCache(const engine::data::HashedString& i_file_name) const
{
    return GetFileFromCache(i_file_name.GetHash());
}

inline const FileUtils::FileData FileUtils::GetFileFromCache(unsigned int i_hash) const
{
    std::lock_guard<std::mutex> lock(file_cache_mutex_);
    const auto it = file_cache_.find(i_hash);
    return it != file_cache_.end() ? it->second : FileData();
}

inline bool FileUtils::IsFileCached(const engine::data::PooledString& i_file_name) const
{
    return IsFileCached(engine::data::HashedString::Hash(i_file_name));
}

inline bool FileUtils::IsFileCached(const engine::data::HashedString& i_file_name) const
{
    return IsFileCached(i_file_name.GetHash());
}

inline bool FileUtils::IsFileCached(unsigned int i_hash) const
{
    std::lock_guard<std::mutex> lock(file_cache_mutex_);
    return file_cache_.find(i_hash) != file_cache_.end();
}

} // namespace util
} // namespace engine
