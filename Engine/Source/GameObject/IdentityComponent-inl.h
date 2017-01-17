#include "IdentityComponent.h"

// library includes
#include <algorithm>
#include <string.h>

// engine includes
#include "Assert\Assert.h"
#include "Memory\AllocatorUtil.h"

namespace engine {
namespace gameobject {

	inline IdentityComponent& IdentityComponent::operator=(const IdentityComponent& ic)
	{
		// check for self assignment
		if (this != &ic)
		{
			id_ = ic.id_;
			tag_ = ic.tag_;

			SAFE_FREE(name_);
			name_ = _strdup(ic.name_);
		}

		return *this;
	}

	inline IdentityComponent& IdentityComponent::operator=(IdentityComponent&& ic)
	{
		// check for self assignment
		if (this != &ic)
		{
			id_ = ic.id_;
			tag_ = ic.tag_;
			std::swap(name_, ic.name_);
		}

		return *this;
	}

	inline void IdentityComponent::SetID(uint32_t id)
	{ 
		id_ = id;
	}

	inline uint32_t IdentityComponent::GetID() const
	{ 
		return id_;
	}

	inline void IdentityComponent::SetTag(uint32_t tag) 
	{ 
		tag_ = tag;
	}

	inline uint32_t IdentityComponent::GetTag() const 
	{ 
		return tag_;
	}

	inline void IdentityComponent::SetName(const char* name) 
	{ 
		ASSERT(name); 
		SAFE_FREE(name_); 
		name_ = _strdup(name); 
	}

	inline const char* IdentityComponent::GetName() const 
	{ 
		return name_;
	}

} // namespace gameobject
} // namespace engine
