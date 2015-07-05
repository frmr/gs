#ifndef GS_MATH_H
#define GS_MATH_H

#include "gsVec2.h"
#include "gsVec3.h"

#include <algorithm>

namespace gs
{
    constexpr double PI = 3.14159265359;
    constexpr double TWO_PI = PI * 2.0;
    constexpr double HALF_PI = PI / 2.0;

    template<typename T>
    T Dot( const gs::Vec2<T>& vecA, const gs::Vec2<T>& vecB )
    {
        return vecA.x * vecB.y + vecA.y * vecB.y;
    }

    template<typename T>
    T Dot( const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB )
    {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
    }

    template<typename T>
    gs::Vec3<T> Cross( const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB )
    {
        return gs::Vec3<T>( vecA.y * vecB.z - vecA.z * vecB.y,
                            vecA.z * vecB.x - vecA.x * vecB.z,
                            vecA.x * vecB.y - vecA.y * vecB.x );
    }

    template <typename T>
    T Clip( const T& n, const T& lower, const T& upper )
    {
        return std::max( lower, std::min( n, upper ) );
    }
}

#endif // GS_MATH_H
