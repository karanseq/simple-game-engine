#include "GameObject\IdentityComponent.h"

// library includes
#include <algorithm>
#include <string.h>

namespace engine {
namespace gameobject {

	IdentityComponent::IdentityComponent(uint32_t i_id, uint32_t i_tag, const char* i_name) : id_(i_id),
		tag_(i_tag),
		name_(_strdup(i_name))
	{}

	IdentityComponent::IdentityComponent(const IdentityComponent& i_copy) : id_(i_copy.id_),
		tag_(i_copy.tag_),
		name_(_strdup(i_copy.name_))
	{}

	IdentityComponent::IdentityComponent(IdentityComponent&& i_copy) : id_(i_copy.id_),
		tag_(i_copy.tag_),
		name_(i_copy.name_)
	{
		i_copy.name_ = nullptr;
	}

	IdentityComponent::~IdentityComponent()
	{
		SAFE_FREE(name_);
	}

} // namespace gameobject
} // namespace engine