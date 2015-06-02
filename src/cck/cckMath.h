#ifndef CCK_MATH_H
#define CCK_MATH_H

#include "cckVec3.h"
#include "cckGeoCoord.h"

namespace cck
{
    static constexpr double pi = 3.141592653589793238462643383279502884;
    static constexpr double halfPi = pi / 2.0;
    static constexpr double twoPi = pi * 2.0;

    cck::Vec3               CrossProduct( const cck::Vec3& vecA, const cck::Vec3& vecB );
    double                  DotProduct( const cck::Vec3& vecA, const cck::Vec3& vecB );

    double                  Distance( const cck::GeoCoord& coordA, const cck::GeoCoord& coordB, const double radius );
}

#endif // CCK_MATH_H
