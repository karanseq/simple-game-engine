#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

// library includes
#include <unordered_map>

// engine includes
#include "Data\PooledString.h"

namespace engine {
namespace util {

class FileUtils
{
public:
	struct FileData {
		engine::data::PooledString				file_name;
		uint8_t*								file_contents;
		size_t									file_size;

		explicit FileData() : file_name(""),
			file_contents(nullptr),
			file_size(0)
		{}

		explicit FileData(const engine::data::PooledString& i_file_name, uint8_t* i_file_contents, size_t i_file_size) : file_name(i_file_name),
			file_contents(i_file_contents),
			file_size(i_file_size)
		{}

		FileData(const FileData& i_copy) : file_name(i_copy.file_name),
			file_contents(i_copy.file_contents),
			file_size(i_copy.file_size)
		{}

		FileData& operator=(const FileData& i_other)
		{
			if (this != &i_other)
			{
				file_name = i_other.file_name;
				file_contents = i_other.file_contents;
				file_size = i_other.file_size;
			}
			return *this;
		}

	}; // struct FileData

private:
	FileUtils();
	~FileUtils();
	static FileUtils* instance_;

	// disable copy constructor & copy assignment operator
	FileUtils(const FileUtils& i_copy) = delete;
	FileUtils& operator=(const FileUtils& i_copy) = delete;

public:
	static FileUtils* Create();
	static void Destroy();
	static inline FileUtils* Get();

	FileData ReadFile(const engine::data::PooledString& i_file_name, bool i_cache_file);
	
	bool WriteFile(const engine::data::PooledString& i_file_name, const char* i_file_contents) const;
	void ClearFileCache();

	inline bool IsFileCached(const engine::data::PooledString& i_file_name) const;
	inline bool IsFileCached(unsigned int i_hash) const;

private:
	std::unordered_map<unsigned int, FileData>				file_cache_;
	
}; // class FileUtils

} // namespace util
} // namespace engine

#include "FileUtils-inl.h"

#endif // FILE_UTILS_H_
