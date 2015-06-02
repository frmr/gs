#include "cckMath.h"

#include <cmath>

cck::Vec3 cck::CrossProduct( const cck::Vec3& vecA, const cck::Vec3& vecB )
{
    return cck::Vec3( vecA.y * vecB.z - vecA.z * vecB.y,
                      vecA.z * vecB.x - vecA.x * vecB.z,
                      vecA.x * vecB.y - vecA.y * vecB.x );
}

double cck::DotProduct( const cck::Vec3& vecA, const cck::Vec3& vecB )
{
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

double cck::Distance( const GeoCoord& coordA, const GeoCoord& coordB, const double radius )
{
    return radius * acos( sin( coordA.latRadians ) * sin( coordB.latRadians ) + cos( coordA.latRadians ) * cos( coordB.latRadians ) * cos( coordB.lonRadians - coordA.lonRadians ) );
}
