#ifndef HASHED_STRING_H_
#define HASHED_STRING_H_

#include "PooledString.h"

namespace engine {
namespace data {

class HashedString
{
public:
	HashedString();
	explicit HashedString(const char* i_string);
	~HashedString();

	HashedString(const HashedString& i_copy);
	HashedString(const PooledString& i_string);
	
	inline HashedString& operator=(const HashedString& i_copy);
	inline bool operator==(const HashedString& i_other) const;
	inline bool operator!=(const HashedString& i_other) const;
	inline bool operator<(const HashedString& i_other) const;
	inline bool operator>(const HashedString& i_other) const;
	inline unsigned int GetHash() const;

	static inline unsigned int Hash(const char* i_string);
	static inline unsigned int Hash(const PooledString& i_string);

private:
	static unsigned int Hash(const void* i_bytes, unsigned int i_byte_count);

	unsigned int								hash_;

}; // class HashedString

} // namespace data
} // namespace engine

#include "HashedString-inl.h"

#endif // HASHED_STRING_H_
