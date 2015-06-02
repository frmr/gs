#ifndef CCK_GEO_COORD_H
#define CCK_GEO_COORD_H

#include "cckVec3.h"

namespace cck
{
    class Vec3;

    class GeoCoord
    {
    public:
        cck::Vec3       ToCartesian( const double radius ) const;

    public:
        const double    latRadians;
        const double    lonRadians;

    public:
        GeoCoord( const double latRadians, const double lonRadians );
    };
}

#endif // CCK_GEO_COORD_H
