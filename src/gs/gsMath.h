#ifndef GS_MATH_H
#define GS_MATH_H

#include "gsVec2.h"
#include "gsVec3.h"

#include <algorithm>
#include <cmath>

namespace gs
{
    constexpr double PI = 3.14159265359;
    constexpr double TWO_PI = PI * 2.0;
    constexpr double HALF_PI = PI / 2.0;

    template<typename T>
    T Dot(const gs::Vec2<T>& vecA, const gs::Vec2<T>& vecB)
    {
        return vecA.x * vecB.x + vecA.y * vecB.y;
    }

    template<typename T>
    T Dot(const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB)
    {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
    }

    template<typename T>
    gs::Vec3<T> Cross(const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB)
    {
        return gs::Vec3<T>(vecA.y * vecB.z - vecA.z * vecB.y,
                            vecA.z * vecB.x - vecA.x * vecB.z,
                            vecA.x * vecB.y - vecA.y * vecB.x);
    }

    template <typename T>
    T Clip(const T& n, const T& lower, const T& upper)
    {
        return std::max(lower, std::min(n, upper));
    }

//    template <typename V>
//    V ClosestPointOnLine(const V& a, const V& ab, const V& p, const bool clamp)
//    {
//        V ap = p - a;
//
//        const double ab2 = gs::Dot(ab, ab);
//        const double t = gs::Dot(ap, ab) / ab2;
//        if (clamp)
//        {
//            if (t < 0.0)
//            {
//                t = 0.0;
//            }
//            else if (t > 1.0)
//            {
//                t = 1.0;
//            }
//        }
//        return a + ab * t;
//    }

    template <typename V>
    double ClosestPointOnLine(const V& a, const V& ab, const V& p, const bool clamp)
    {
        V ap = p - a;

        const double ab2 = gs::Dot(ab, ab);
        const double t = gs::Dot(ap, ab) / ab2;
        if (clamp)
        {
            if (t < 0.0)
            {
                t = 0.0;
            }
            else if (t > 1.0)
            {
                t = 1.0;
            }
        }
        return t;
    }
}

#endif // GS_MATH_H
