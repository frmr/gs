#include "voronoi.h"

#include <iostream>
#include <math.h>
#include <algorithm>


Voronoi::Voronoi()
{
    sweepline_large = 0.0;
    sweepline_small = 0.0;
}

void Voronoi::initializeSiteQueue(std::vector<VoronoiCell*> cells)
{
    for (std::vector<VoronoiCell*>::iterator it = cells.begin(); it != cells.end(); ++it)
    {
        VoronoiCell* b = *it;
        SiteEvent* se = new SiteEvent(b->position, b);
        sites.push_back(se);
    }
    std::sort(sites.begin(), sites.end(), voronoi_event_comp);
}

void Voronoi::construct(std::vector<VoronoiCell*> & cells)
{
    initializeSiteQueue(cells);

    // pop events from sites and circles in order of highest priority
    while (sites.size() || !circles.empty())
    {
        if (circles.empty())
        {
            SiteEvent* next_site = sites.back();
            sites.pop_back();
            processSiteEvent(next_site);
        }
        else if (!sites.size())
        {
            CircleEvent* next_circle = circles.top();
            circles.pop();
            processCircleEvent(next_circle);
        }
        else
        {
            SiteEvent* next_site = sites.back();
            CircleEvent* next_circle = circles.top();


            if (voronoi_event_comp((VoronoiEvent*)next_circle,(VoronoiEvent*)next_site))
            {
                sites.pop_back();
                processSiteEvent(next_site);
            }
            else
            {
                circles.pop();
                processCircleEvent(next_circle);
            }
        }
    }
    if (bl.getSize() != 2) std::cout << "ERROR - beachline size: " << bl.getSize() << "\n";
}

void Voronoi::processSiteEvent(SiteEvent* site)
{
    SkipNode* node = bl.insert(site, site->polar);

    // manage circle events
    if (bl.getSize() > 2)
    {
        removeCircleEvent(node->prev->site);
        addCircleEventProcessSite(node->prev);
        addCircleEventProcessSite(node->next);
    }
}

// creates a voronoi vertex
void Voronoi::processCircleEvent(CircleEvent* circle)
{
    sweepline_large = circle->polar;
    sweepline_small = circle->polar_small;

    SkipNode* sn = circle->node;
    SkipNode* sni = sn->prev;
    SkipNode* snk = sn->next;

    SiteEvent* si = sni->site;
    SiteEvent* sj = sn->site;
    SiteEvent* sk = snk->site;

    // add vertex to cells
    si->cell->addCorner(circle->center.to_dvec3());
    sj->cell->addCorner(circle->center.to_dvec3());
    sk->cell->addCorner(circle->center.to_dvec3());

    // remove circle events of neighbors
    removeCircleEvent(si);
    removeCircleEvent(sk);

    // remove site from beachline
    bl.erase(sn);

    // check for new circle events
    addCircleEventProcessCircle(sni);
    addCircleEventProcessCircle(snk);

    // free memory
    delete circle;
}

mpvec3 Voronoi::circumcenter(mpvec3 i, mpvec3 j, mpvec3 k)
{
    return mpvec3::normalize(mpvec3::cross((i-j),(k-j)));
}

long double Voronoi::circleBottom(mpvec3 cc, mpvec3 p)
{
    return acos(cc.z) + acos(mpvec3::dot(cc,p));
}

void Voronoi::addCircleEventProcessSite(SkipNode* node)
{
    SiteEvent* s1 = node->prev->site;
    SiteEvent* s2 = node->site;
    SiteEvent* s3 = node->next->site;

    mpvec3 cc = circumcenter(s1->point, s2->point, s3->point);
    long double small_polar = acos(mpvec3::dot(cc,s2->point));
    long double large_polar = acos(cc.z);

    addCircleEvent(node, large_polar, small_polar, cc);
}

void Voronoi::addCircleEventProcessCircle(SkipNode* node)
{
    SiteEvent* s1 = node->prev->site;
    SiteEvent* s2 = node->site;
    SiteEvent* s3 = node->next->site;
    mpvec3 cc = circumcenter(s1->point, s2->point, s3->point);
    long double small_polar = acos(mpvec3::dot(cc,s2->point));
    long double large_polar = acos(cc.z);

    if ((large_polar-sweepline_large) + (small_polar-sweepline_small) >= 0) addCircleEvent(node, large_polar, small_polar, cc);
}

inline void Voronoi::addCircleEvent(SkipNode* node, long double large_polar, long double small_polar, mpvec3 cc)
{
    CircleEvent* ce = new CircleEvent(large_polar, small_polar, cc, node);
    circles.push(ce);
    node->site->ce = ce;
}

void Voronoi::removeCircleEvent(SiteEvent* site)
{
    if (site->ce != NULL)
    {
        circles.erase(site->ce->pnode);
        delete site->ce;
        site->ce = NULL;
    }
}
