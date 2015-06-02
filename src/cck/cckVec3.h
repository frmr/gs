#ifndef CCK_VEC3_H
#define CCK_VEC3_H

#include "cckGeoCoord.h"

namespace cck
{
    class GeoCoord;

    class Vec3
    {
    public:
        double          x, y, z;

    public:
        cck::GeoCoord   ToGeographic() const;
        Vec3            Unit() const;

        Vec3            operator+( const Vec3& rhs ) const;
        Vec3            operator-( const Vec3& rhs ) const;
        Vec3            operator*( const double& rhs ) const;
        Vec3            operator/( const double& rhs ) const;

        Vec3&           operator+=( const Vec3& rhs );
        Vec3&           operator-=( const Vec3& rhs );
        Vec3&           operator*=( const double& rhs );
        Vec3&           operator/=( const double& rhs );

    public:
        Vec3();
        Vec3( const double x, const double y, const double z );
    };
}
#endif // CCK_VEC3_H
