#ifndef ENGINE_VEC2D_H_
#define ENGINE_VEC2D_H_

namespace engine {
namespace math {

	/*
		Vec2D
		- A class that represents a 2D vector
		- Overloads basic arithmetic operators, validates against bad floats
	*/

	class Vec2D
	{
	public:
		explicit Vec2D(float i_x = 0.0f, float i_y = 0.0f);
		Vec2D(const Vec2D& i_copy);

		~Vec2D()
		{}

		// accessors and mutators
		inline float x() const;
		inline void x(float i_x);
		inline float y() const;
		inline void y(float i_y);
		inline void set(float i_x = 0.0f, float i_y = 0.0f);

		// assignment
		inline Vec2D& operator=(const Vec2D& i_vec);

		// arithmetic
		inline Vec2D operator+(const Vec2D& i_vec) const;
		inline Vec2D& operator+=(const Vec2D& i_vec);
		inline Vec2D operator-(const Vec2D& i_vec) const;
		inline Vec2D& operator-=(const Vec2D& i_vec);
		inline Vec2D operator*(float i_scale) const;
		inline Vec2D& operator*=(float i_scale);

		// relational
		inline bool operator==(const Vec2D& i_vec) const;
		inline bool operator!=(const Vec2D& i_vec) const;

		// unary
		inline Vec2D operator-() const;

		// functions
		inline bool IsZero() const;
		inline bool IsOne() const;
		inline float LengthSquared() const;
		float Length() const;
		void Normalize();
		Vec2D Normalize() const;

		// constants
		static const Vec2D			ZERO;
		static const Vec2D			UNIT;
		static const Vec2D			UNIT_X;
		static const Vec2D			UNIT_Y;

	protected:
		float						x_;
		float						y_;
	}; // class Vec2D

} // namespace math
} // namespace engine

#include "Vec2D-inl.h"

#endif // ENGINE_VEC2D_H_