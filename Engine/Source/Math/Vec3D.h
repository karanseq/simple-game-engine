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
		explicit Vec3D(float i_x = 0.0f, float i_y = 0.0f, float i_z = 0.0f);
		Vec3D(const Vec3D& i_copy);

		~Vec3D()
		{}

		// accessors and mutators
		inline float x() const;
		inline void x(float i_x);
		inline float y() const;
		inline void y(float i_y);
		inline float z() const;
		inline void z(float i_z);
		inline void set(float i_x = 0.0f, float i_y = 0.0f, float i_z = 0.0f);

		// assignment
		inline Vec3D& operator=(const Vec3D& i_vec);

		// arithmetic
		inline Vec3D operator+(const Vec3D& i_vec) const;
		inline Vec3D& operator+=(const Vec3D& i_vec);
		inline Vec3D operator-(const Vec3D& i_vec) const;
		inline Vec3D& operator-=(const Vec3D& i_vec);
		inline Vec3D operator*(float i_scale) const;
		inline Vec3D& operator*=(float i_scale);

		// relational
		inline bool operator==(const Vec3D& i_vec) const;
		inline bool operator!=(const Vec3D& i_vec) const;

		// unary
		inline Vec3D operator-() const;

		// functions
		inline bool IsZero() const;
		inline bool IsOne() const;
		inline float LengthSquared() const;
		float Length() const;
		void Normalize();
		Vec3D Normalize() const;

		// constants
		static const Vec3D			ZERO;
		static const Vec3D			UNIT;
		static const Vec3D			UNIT_X;
		static const Vec3D			UNIT_Y;
		static const Vec3D			UNIT_Z;

	protected:
		float						x_;
		float						y_;
		float						z_;
	}; // class Vec3D

} // namespace math
} // namespace engine

#include "Vec3D-inl.h"

#endif // ENGINE_VEC3D_H_
