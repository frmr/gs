#include "cckVec3.h"

#include "cckMath.h"
#include <cmath>

cck::GeoCoord cck::Vec3::ToGeographic() const
{
    double lonRadians = atan2( y, x );

    if ( lonRadians < -cck::pi )
    {
        lonRadians += cck::twoPi;
    }
    else if ( lonRadians > cck::pi )
    {
        lonRadians -= cck::twoPi;
    }

    return cck::GeoCoord( atan2( z, sqrt( x * x + y * y ) ), lonRadians );
}

cck::Vec3 cck::Vec3::Unit() const
{
    const double length = sqrt( x * x + y * y + z * z );
    return cck::Vec3( x / length, y / length, z / length );
}

cck::Vec3 cck::Vec3::operator+( const Vec3& rhs ) const
{
    return cck::Vec3( x + rhs.x, y + rhs.y, z + rhs.z );
}

cck::Vec3 cck::Vec3::operator-( const Vec3& rhs ) const
{
    return cck::Vec3( x - rhs.x, y - rhs.y, z - rhs.z );
}

cck::Vec3 cck::Vec3::operator*( const double& rhs ) const
{
    return cck::Vec3( x * rhs, y * rhs, z * rhs );
}

cck::Vec3 cck::Vec3::operator/( const double& rhs ) const
{
    return cck::Vec3( x / rhs, y / rhs, z / rhs );
}

cck::Vec3& cck::Vec3::operator+=( const Vec3& rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

cck::Vec3& cck::Vec3::operator-=( const Vec3& rhs )
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

cck::Vec3& cck::Vec3::operator*=( const double& rhs )
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

cck::Vec3& cck::Vec3::operator/=( const double& rhs )
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

cck::Vec3::Vec3()
    :   x( 0.0 ), y( 0.0 ), z( 0.0 )
{
}

cck::Vec3::Vec3( const double x, const double y, const double z )
    :   x( x ), y( y ), z( z )
{
}
