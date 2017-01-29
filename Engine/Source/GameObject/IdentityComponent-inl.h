#include "IdentityComponent.h"

// library includes
#include <algorithm>
#include <string.h>

// engine includes
#include "Assert\Assert.h"
#include "Common\HelperMacros.h"

namespace engine {
namespace gameobject {

	inline IdentityComponent& IdentityComponent::operator=(const IdentityComponent& i_ic)
	{
		// check for self assignment
		if (this != &i_ic)
		{
			id_ = i_ic.id_;
			tag_ = i_ic.tag_;

			SAFE_FREE(name_);
			name_ = _strdup(i_ic.name_);
		}

		return *this;
	}

	inline IdentityComponent& IdentityComponent::operator=(IdentityComponent&& i_ic)
	{
		// check for self assignment
		if (this != &i_ic)
		{
			id_ = i_ic.id_;
			tag_ = i_ic.tag_;
			std::swap(name_, i_ic.name_);
		}

		return *this;
	}

	inline void IdentityComponent::SetID(uint32_t i_id)
	{ 
		id_ = i_id;
	}

	inline uint32_t IdentityComponent::GetID() const
	{ 
		return id_;
	}

	inline void IdentityComponent::SetTag(uint32_t i_tag) 
	{ 
		tag_ = i_tag;
	}

	inline uint32_t IdentityComponent::GetTag() const 
	{ 
		return tag_;
	}

	inline void IdentityComponent::SetName(const char* i_name) 
	{ 
		ASSERT(i_name); 
		SAFE_FREE(name_); 
		name_ = _strdup(i_name); 
	}

	inline const char* IdentityComponent::GetName() const 
	{ 
		return name_;
	}

} // namespace gameobject
} // namespace engine
