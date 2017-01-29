#ifndef SHARED_POINTER_H_
#define SHARED_POINTER_H_

// engine includes
#include "Memory\RefCounter.h"

namespace engine {
namespace memory {

// forward declaration
template<class T>
class WeakPointer;

template<class T>
class SharedPointer
{
public:
	SharedPointer(T* i_object = nullptr) : object_(i_object),
		ref_counter_(nullptr)
	{
		if (object_)
		{
			ref_counter_ = new RefCounter(1);
		}
	};
	~SharedPointer()
	{
		Release();
	};

	SharedPointer(const SharedPointer& i_copy) : object_(i_copy.object_),
		ref_counter_(i_copy.ref_counter_)
	{
		Acquire();
	}

	SharedPointer(SharedPointer&& i_copy) : object_(i_copy.object_),
		ref_counter_(i_copy.ref_counter_)
	{
		i_copy.object_ = nullptr;
		i_copy.ref_counter_ = nullptr;
	}

	SharedPointer(const WeakPointer<T>& i_weak_pointer) : object_(i_weak_pointer.HasExpired() ? nullptr : i_weak_pointer.object_),
		ref_counter_(i_weak_pointer.HasExpired() ? nullptr : i_weak_pointer.ref_counter_)
	{
		Acquire();
	}
	
	inline SharedPointer& operator=(const SharedPointer& i_copy);
	inline SharedPointer& operator=(SharedPointer&& i_copy);

	inline T* operator->() const;
	inline T& operator*() const;

	inline operator bool() const;

	inline bool operator==(const SharedPointer& i_other) const;
	inline bool operator!=(const SharedPointer& i_other) const;

#ifdef BUILD_DEBUG
	inline long GetStrongCount() const;
	inline long GetWeakCount() const;
#endif // BUILD_DEBUG

private:
	inline void Acquire();
	inline void Release();

private:
	T*								object_;
	RefCounter*						ref_counter_;

	template<class T>
	friend class WeakPointer;
}; // class StrongPointer

} // namespace memory
} // namespace engine

#include "SharedPointer-inl.h"

#endif // SHARED_POINTER_H_
