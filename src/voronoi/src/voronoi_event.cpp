#include "voronoi_event.h"

#include <math.h>

VoronoiEvent::VoronoiEvent()
{
    azimuth = 0.0;
    polar = 0.0;
    point = mpvec3();

    polar_small = 0.0;
}

VoronoiEvent::VoronoiEvent(mpvec3 p)
{
    polar = acos(p.z);

    const long double PI = M_PIl;
    azimuth = atan2(p.y, p.x);
    azimuth /= 2.0 * PI;
    azimuth -= floor(azimuth);
    azimuth *= 2.0 * PI;

    point = p;

    polar_small = 0.0;
}

SiteEvent::SiteEvent(mpvec3 p, VoronoiCell* b) : VoronoiEvent(p)
{
    cell = b;
    ce = NULL;
}

SiteEvent::SiteEvent(SiteEvent* copy) : VoronoiEvent(copy->point)
{
    cell = copy->cell;
    ce = NULL;
}

CircleEvent::CircleEvent(long double polar, long double polar_small, mpvec3 c, SkipNode* nd)
{
    this->polar = polar;
    this->polar_small = polar_small;

    const long double PI = M_PIl;
    azimuth = atan2(c.y, c.x);
    azimuth /= 2.0 * PI;
    azimuth -= floor(azimuth);
    azimuth *= 2.0 * PI;

    center = c;
    node = nd;
    pnode = NULL;
}
