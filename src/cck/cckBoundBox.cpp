#include "cckBoundBox.h"

bool cck::BoundBox::Contains( const double latitude, const double longitude ) const
{
    return  latitude >= minCoord.latRadians &&
            latitude <= maxCoord.latRadians &&
            longitude >= minCoord.lonRadians &&
            longitude <= maxCoord.lonRadians;
}

bool cck::BoundBox::Contains( const cck::GeoCoord& coord ) const
{
    return  coord.latRadians >= minCoord.latRadians &&
            coord.latRadians <= maxCoord.latRadians &&
            coord.lonRadians >= minCoord.lonRadians &&
            coord.lonRadians <= maxCoord.lonRadians;
}

cck::BoundBox::BoundBox( const cck::GeoCoord& minCoord, const cck::GeoCoord& maxCoord )
    :   minCoord( minCoord ),
        maxCoord( maxCoord )
{
}
