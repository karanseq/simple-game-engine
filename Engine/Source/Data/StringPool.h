#ifndef STRING_POOL_H_
#define STRING_POOL_H_

// library includes
#include <mutex>
#include <stdint.h>

namespace engine {
namespace data {

// TODO: Add support for multiple pools
class StringPool
{
public:
    static StringPool* Create(size_t i_bytes = DEFAULT_POOL_SIZE);
    static void Destroy();
    static inline StringPool* Get();

    const char* Add(const char* i_string);
    const char* Find(const char* i_string);

    // constants
    static const size_t                             DEFAULT_POOL_SIZE;

private:
    explicit StringPool(uint8_t* i_pool, size_t i_pool_size);
    ~StringPool();
    static StringPool* instance_;

    // disable copy constructor & copy assignment operator
    StringPool(const StringPool& i_copy) = delete;
    StringPool& operator=(const StringPool&) = delete;

    inline bool Contains(const char* i_string) const;
#ifdef BUILD_DEBUG
    void DumpStatistics() const;
#endif

    uint8_t*                                        pool_;
    uint8_t*                                        pool_end_;
    size_t                                          pool_size_;
    mutable std::mutex                              pool_mutex_;
#ifdef BUILD_DEBUG
    size_t                                          memory_used_;
    size_t                                          num_strings_;
#endif
};

} // namespace data
} // namespace engine

#include "StringPool-inl.h"

#endif // STRING_POOL_H_