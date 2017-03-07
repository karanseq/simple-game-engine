#ifndef RECT_H_
#define RECT_H_

#include "Vec2D.h"
#include "Size.h"

namespace engine {
namespace math {

    /*
        Rect
        - A class that represents a rectangle
        - Overloads basic arithmetic operators, validates against bad floats
    */

    class Rect
    {
    public:
        explicit Rect(float i_x = 0.0f, float i_y = 0.0f, float i_width = 0.0f, float i_height = 0.0f);
        explicit Rect(const Vec2D& i_origin = Vec2D::ZERO, const Size& i_size = Size::ZERO);
        Rect(const Rect& i_copy);

        ~Rect()
        {}

        // accessors and mutators
        inline void SetRect(float i_x, float i_y, float i_width, float i_height);
        inline float GetMinX() const;
        inline float GetMinY() const;
        inline float GetMaxX() const;
        inline float GetMaxY() const;

        // assignment
        inline Rect& operator=(const Rect& i_copy);

        // relational
        inline bool operator==(const Rect& i_rect) const;
        inline bool operator!=(const Rect& i_rect) const;

        // functions
        inline bool Equals(const Rect& i_rect) const;
        inline bool ContainsPoint(const Vec2D& i_point) const;
        inline bool IntersectsRect(const Rect& i_rect) const;

        // constants
        static const Rect           ZERO;

    protected:
        Vec2D                       origin_;
        Size                        size_;
    }; // class Rect

} // namespace math
} // namespace engine

#include "Rect-inl.h"

#endif // RECT_H_