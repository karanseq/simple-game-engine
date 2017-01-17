#ifndef ENGINE_VEC3D_H_
#define ENGINE_VEC3D_H_

namespace engine {
namespace math {

	/*
		Vec3D
		- A class that represents a 3D vector
		- Overloads basic arithmetic operators, validates against bad floats
	*/

	class Vec3D
	{
	public:
		Vec3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		Vec3D(const Vec3D& copy);

		~Vec3D()
		{}

		// accessors and mutators
		inline float x() const;
		inline void x(float x);
		inline float y() const;
		inline void y(float y);
		inline float z() const;
		inline void z(float z);
		inline void set(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		// assignment
		inline Vec3D& operator=(const Vec3D& vec);

		// arithmetic
		inline Vec3D operator+(const Vec3D& vec) const;
		inline Vec3D& operator+=(const Vec3D& vec);
		inline Vec3D operator-(const Vec3D& vec) const;
		inline Vec3D& operator-=(const Vec3D& vec);
		inline Vec3D& operator*=(float scale);

		// relational
		inline bool operator==(const Vec3D& vec) const;
		inline bool operator!=(const Vec3D& vec) const;

		// unary
		inline Vec3D operator-() const;

		// constants
		static const Vec3D ZERO;
		static const Vec3D UNIT;
		static const Vec3D UNIT_X;
		static const Vec3D UNIT_Y;
		static const Vec3D UNIT_Z;

	protected:
		float x_;
		float y_;
		float z_;
	}; // class Vec3D

} // namespace math
} // namespace engine

#include "Vec3D-inl.h"

#endif // ENGINE_VEC3D_H_
