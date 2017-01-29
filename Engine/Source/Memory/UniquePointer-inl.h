#include "UniquePointer.h"

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace memory {

template<class T>
inline UniquePointer<T>& UniquePointer<T>::operator=(UniquePointer&& i_copy)
{
	if (this != &i_copy)
	{
		std::swap(object_, i_copy.object_);
	}
	return *this;
}

template<class T>
inline T* UniquePointer<T>::operator->() const
{
    return object_;
}

template<class T>
inline T& UniquePointer<T>::operator*() const
{
    ASSERT(object_);
    return *object_;
}

template<class T>
inline UniquePointer<T>::operator bool() const
{
    return (object_ != nullptr);
}

template<class T>
inline bool UniquePointer<T>::operator==(const UniquePointer& i_other) const
{
    return (object_ == i_other.object_);
}

template<class T>
inline bool UniquePointer<T>::operator!=(const UniquePointer& i_other) const
{
    return (object_ != i_other.object_);
}

} // namespace memory
} // namespace engine
