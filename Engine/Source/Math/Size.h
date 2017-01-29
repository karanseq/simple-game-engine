#ifndef SIZE_H_
#define SIZE_H_

namespace engine {
namespace math {

	/*
		Size
		- A class that represents the 2D dimensions of an object
		- Overloads basic arithmetic operators, validates against bad floats
	*/

	class Size
	{
	public:
		explicit Size(float i_width = 0.0f, float i_height = 0.0f);
		Size(const Size& i_copy);

		~Size()
		{}

		// accessors and mutators
		inline float width() const;
		inline void width(float i_width);
		inline float height() const;
		inline void height(float i_height);
		inline void set(float i_width, float i_height);

		// assignment
		inline Size& operator=(const Size& i_copy);

		// arithmetic
		inline Size operator+(const Size& i_size) const;
		inline Size operator-(const Size& i_size) const;
		inline Size operator*(float i_factor) const;
		inline Size operator/(float i_factor) const;

		// relational
		inline bool operator==(const Size& i_size) const;
		inline bool operator!=(const Size& i_size) const;

		// constants
		static const Size			ZERO;

	protected:
		float						width_;
		float						height_;
	}; // class Size

} // namespace math
} // namespace engine

#include "Size-inl.h"

#endif // SIZE_H_