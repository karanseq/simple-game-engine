#ifndef ENGINE_IDENTITY_COMPONENT_H_
#define ENGINE_IDENTITY_COMPONENT_H_

// library includes
#include <stdint.h>

namespace engine {
namespace gameobject {

	/*
		IdentityComponent
		- A component that can be used to uniquely identify any object within the engine
	*/

	class IdentityComponent
	{
	public:
		explicit IdentityComponent(uint32_t i_id = 0, uint32_t i_tag = 0, const char* i_name = "");
	
		// standard copy constuctor
		IdentityComponent(const IdentityComponent& i_copy);

		// move copy constructor
		IdentityComponent(IdentityComponent&& i_copy);

		~IdentityComponent();

		// copy assignment
		inline IdentityComponent& operator=(const IdentityComponent& i_ic);

		// move assignment
		inline IdentityComponent& operator=(IdentityComponent&& i_ic);

		// accessors and mutators
		inline void SetID(uint32_t i_id);
		inline uint32_t GetID() const;

		inline void SetTag(uint32_t i_tag);
		inline uint32_t GetTag() const;

		inline void SetName(const char* i_name);
		inline const char* GetName() const;

	protected:
		uint32_t			id_;
		uint32_t			tag_;
		char*				name_;
	}; // class IdentityComponent

} // namespace gameobject
} // namespace engine

#include "IdentityComponent-inl.h"

#endif // ENGINE_IDENTITY_COMPONENT_H_
