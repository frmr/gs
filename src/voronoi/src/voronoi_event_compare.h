#ifndef VORONOI_EVENT_COMPARE_H
#define VORONOI_EVENT_COMPARE_H

#include "voronoi_event.h"

class VoronoiEvent;

struct VoronoiEventCompare
{
    bool operator()(VoronoiEvent* lhs, VoronoiEvent* rhs)
    {
        if ((lhs->polar - rhs->polar) + (lhs->polar_small - rhs->polar_small) == 0.0)
            return (lhs->azimuth > rhs->azimuth);
        else
            return ((lhs->polar - rhs->polar) + (lhs->polar_small - rhs->polar_small) > 0.0);
    }
};

#endif