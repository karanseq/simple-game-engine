#include "FileUtils.h"

// engine includes
#include "Data\HashedString.h"

namespace engine {
namespace util {

inline FileUtils* FileUtils::Get()
{
	return FileUtils::instance_;
}

inline bool FileUtils::IsFileCached(const engine::data::PooledString& i_file_name) const
{
	return IsFileCached(engine::data::HashedString::Hash(i_file_name));
}

inline bool FileUtils::IsFileCached(unsigned int i_hash) const
{
	return file_cache_.count(i_hash) > 0;
}

} // namespace util
} // namespace engine
