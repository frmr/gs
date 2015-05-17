#ifndef VORONOI_EVENT_H
#define VORONOI_EVENT_H

#include <cmath>
#include <list>
#include "../glm/glm.hpp"
#include "voronoi_cell.h"

#include "mpreal/mpvec3.h"

class SkipNode;
class PriQueueNode;

class VoronoiEvent
{
    public:

        VoronoiEvent();
        VoronoiEvent(mpvec3 p);

        long double azimuth, polar;
        mpvec3 point;
        long double polar_small;
};

class CircleEvent;

class SiteEvent : public VoronoiEvent
{
    public:

        SiteEvent(mpvec3 p, VoronoiCell* b);
        SiteEvent(SiteEvent* copy);

        VoronoiCell* cell;
        CircleEvent* ce;
};

class CircleEvent : public VoronoiEvent
{
    public:

        CircleEvent(long double polar, long double polar_, mpvec3 c, SkipNode* nd);

        mpvec3 center;
        SkipNode* node;
        PriQueueNode* pnode;
};

#endif