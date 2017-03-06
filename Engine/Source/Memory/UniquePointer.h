#ifndef UNIQUE_POINTER_H_
#define UNIQUE_POINTER_H_

// engine includes
#include "Common\HelperMacros.h"
#include "Logger\Logger.h"

namespace engine {
namespace memory {

template<class T>
class UniquePointer
{
public:
	UniquePointer() : object_(nullptr)
	{}
	UniquePointer(std::nullptr_t) : object_(nullptr)
	{}
    UniquePointer(T* i_object) : object_(i_object)
    {}
    ~UniquePointer()
    {
        SAFE_DELETE(object_);
	}

	UniquePointer(const UniquePointer& i_copy) = delete;
	UniquePointer& operator=(const UniquePointer& i_other) = delete;

    UniquePointer(UniquePointer&& i_copy) : object_(i_copy.object_)
    {
        i_copy.object_ = nullptr;
    }

    inline UniquePointer& operator=(UniquePointer&& i_copy);

    inline T* operator->() const;
    inline T& operator*() const;

    inline operator bool() const;

    inline bool operator==(const UniquePointer& i_other) const;
    inline bool operator!=(const UniquePointer& i_other) const;

private:
    T*								object_;
}; // class UniquePointer

} // namespace memory
} // namespace engine

#include "UniquePointer-inl.h"

#endif // UNIQUE_POINTER_H_
