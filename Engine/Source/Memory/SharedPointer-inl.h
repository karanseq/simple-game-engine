#include "SharedPointer.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Common\HelperMacros.h"

namespace engine {
namespace memory {

template<class T>
inline SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer& i_copy)
{
	// check for self assignment
	if (this != &i_copy)
	{
		// release previously owned object
		Release();
		
		// copy data
		object_ = i_copy.object_;
		ref_counter_ = i_copy.ref_counter_;

		// acquire newly owned object
		Acquire();
	}
	return *this;
}

template<class T>
inline SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer&& i_copy)
{
	// check for self assignment
	if (this != &i_copy)
	{
		// swap data
		std::swap(object_, i_copy.object_);
		std::swap(ref_counter_, i_copy.ref_counter_);
	}
	return *this;
}

template<class T>
inline T* SharedPointer<T>::operator->() const
{
	return object_;
}

template<class T>
inline T& SharedPointer<T>::operator*() const
{
	ASSERT(object_ != nullptr);
	return *object_;
}

template<class T>
inline SharedPointer<T>::operator bool() const
{
	return (object_ != nullptr);
}

template<class T>
inline bool SharedPointer<T>::operator==(const SharedPointer& i_other) const
{
	return (object_ == i_other.object_);
}

template<class T>
inline bool SharedPointer<T>::operator!=(const SharedPointer& i_other) const
{
	return (object_ != i_other.object_);
}

#ifdef BUILD_DEBUG
template<class T>
inline long SharedPointer<T>::GetStrongCount() const
{
	return ref_counter_ ? ref_counter_->strong_count : 0;
}

template<class T>
inline long SharedPointer<T>::GetWeakCount() const
{
	return ref_counter_ ? ref_counter_->weak_count : 0;
}
#endif

template<class T>
inline void SharedPointer<T>::Acquire()
{
	if (ref_counter_)
	{
		++(ref_counter_->strong_count);
	}
}

template<class T>
inline void SharedPointer<T>::Release()
{
	if (ref_counter_)
	{
		if (--(ref_counter_->strong_count) <= 0)
		{
			SAFE_DELETE(object_);
			if (ref_counter_->weak_count <= 0)
			{
				SAFE_DELETE(ref_counter_);
			}
		}
		return;
	}
}

} // namespace memory
} // namespace engine