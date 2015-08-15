#ifndef CCK_BOUND_BOX_H
#define CCK_BOUND_BOX_H

#include "cckGeoCoord.h"

namespace cck
{
    class BoundBox
    {
    public:
        const cck::GeoCoord minCoord;
        const cck::GeoCoord maxCoord;

    public:
        bool    Contains( const double latitude, const double longitude ) const;
        bool    Contains( const cck::GeoCoord& coord ) const;

    public:
        BoundBox( const cck::GeoCoord& minCoord, const cck::GeoCoord& maxCoord );
    };
}

#endif // CCK_BOUND_BOX_H
