#ifndef VORONOI_H
#define VORONOI_H

#include <vector>
#include <cmath>
#include "../glm/glm.hpp"
#include "voronoi_event.h"
#include "voronoi_cell.h"
#include "mpreal/mpvec3.h"
#include "beachline.h"
#include "voronoi_event_compare.h"
#include "priqueue.h"

//#include "gtest/gtest_prod.h"


class Voronoi
{
    public:

        Voronoi();

        void construct(std::vector<VoronoiCell*> & cells);

    private:

        VoronoiEventCompare voronoi_event_comp;

        long double sweepline_large;
        long double sweepline_small;

        mpvec3 circumcenter(mpvec3 i, mpvec3 j, mpvec3 k);
        long double circleBottom(mpvec3 cc, mpvec3 p);

        BeachLine bl;

        PriQueue circles;

        std::vector<SiteEvent*> sites;

        void initializeSiteQueue(std::vector<VoronoiCell*> cells);

        void processSiteEvent(SiteEvent* site);
        void processCircleEvent(CircleEvent* circle);

        void addCircleEventProcessSite(SkipNode* node);
        void addCircleEventProcessCircle(SkipNode* node);
        inline void addCircleEvent(SkipNode* node, long double lp, long double sp, mpvec3 cc);

        void removeCircleEvent(SiteEvent* site);

        // tests
//        FRIEND_TEST(VoronoiTests, TestIntersect);
//        FRIEND_TEST(VoronoiTests, TestIntersectDegenerateParabola);
//        FRIEND_TEST(VoronoiTests, TestVerifyResult);
//        FRIEND_TEST(VoronoiTests, TestBeachLine);
//        FRIEND_TEST(VoronoiTests, TestCircumcenter);
//        FRIEND_TEST(VoronoiTests, TestCircleBottom);

};

#endif
