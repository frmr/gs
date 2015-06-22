#ifndef GS_MATH_H
#define GS_MATH_H

#include "gsVec2.h"
#include "gsVec3.h"

namespace gs
{
    constexpr double PI = 3.14159265359;
    constexpr double TWO_PI = PI * 2.0;
    constexpr double HALF_PI = PI / 2.0;

    template<typename T>
    T Dot( const gs::Vec2<T>& vecA, const gs::Vec2<T>& vecB );

    template<typename T>
    T Dot( const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB );

    template<typename T>
    gs::Vec3<T> Cross( const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB );
}

#endif // GS_MATH_H
