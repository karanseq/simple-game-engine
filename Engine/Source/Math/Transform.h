#ifndef ENGINE_TRANSFORM_H_
#define ENGINE_TRANSFORM_H_

#include "Vec3D.h"

namespace engine {
namespace math {

	/*
		Transform
		- A class that represents the position, scale and rotation of an object in 3D space
	*/

	class Transform
	{
	public:
		Transform(Vec3D position = Vec3D::ZERO, Vec3D rotation = Vec3D::ZERO, Vec3D scale = Vec3D::UNIT);
		Transform(const Transform& copy);

		~Transform()
		{}

		// assignment
		inline Transform& operator=(const Transform& transform);

		// accessors and mutators
		inline const Vec3D& GetPosition() const;
		inline void SetPosition(const Vec3D& position);
		inline const Vec3D& GetRotation() const;
		inline void SetRotation(const Vec3D& rotation);
		inline const Vec3D& GetScale() const;
		inline void SetScale(const Vec3D& scale);

	protected:
		Vec3D position_;
		Vec3D rotation_;
		Vec3D scale_;

	}; // class Transform

} // namespace math
} // namespace engine

#include "Transform-inl.h"

#endif // ENGINE_TRANSFORM_H_
