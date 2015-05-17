#include "../src/voronoi.h"
#include "../src/beachline.h"
#include "../src/voronoi_event.h"
#include "../src/voronoi_cell.h"
#include "../src/mp_sample_generator.h"
#include "../glm/glm.hpp"
#include <iostream>
#include <cmath>
#include <vector>


TEST(VoronoiTests, TestIntersect)
{
    glm::dvec3 p1 = glm::normalize(glm::dvec3(1.0,1.0,0.0));
    glm::dvec3 p2 = glm::normalize(glm::dvec3(-1.0,1.0,0.0));

    SkipNode sn = SkipNode{NULL};
    long double sweepline = 3.0;

    SiteEvent* s1 = new SiteEvent(p1,NULL);
    SiteEvent* s2 = new SiteEvent(p2,NULL);

    long double in = sn.intersect(s1,s2,sweepline,0.0);
    long double in2 = sn.intersect(s2,s1,sweepline,0.0);

    delete s1;
    delete s2;

    EXPECT_FLOAT_EQ(in, (long double)atan2(1.0, 0.0));
    EXPECT_NE(in, in2);
}

TEST(VoronoiTests, TestIntersectDegenerateParabola)
{
    glm::dvec3 p1 = glm::normalize(glm::dvec3(1.0,1.0,0.5f));
    glm::dvec3 p2 = glm::normalize(glm::dvec3(1.0,1.0,0.0));

    SkipNode sn = SkipNode{NULL};
    long double sweepline = acos(p2.z);

    SiteEvent* s1 = new SiteEvent(p1,NULL);
    SiteEvent* s2 = new SiteEvent(p2,NULL);

    long double in = sn.intersect(s1,s2,sweepline,0.0);
    long double in2 = sn.intersect(s2,s1,sweepline,0.0);

    EXPECT_FLOAT_EQ(in, in2);
    EXPECT_FLOAT_EQ(in, (long double)s2->azimuth);

    delete s1;
    delete s2;
}

TEST(VoronoiTests, TestVerifyResult)
{
    // generate VoronoiCell vector
    std::vector<VoronoiCell*> cells;

    const long double PI = M_PIl;

    int c = 40;
    MPSampleGenerator sample_generator;
    mpvec3* samples = sample_generator.getJitteredSamplesMP(c);

    for (int i = 0; i < c*c; i++)
    {
        mpvec3 sample = samples[i];

        // scale sample into range [0,2pi) x [-1,1]
        sample = mpvec3(2.0*PI*sample.x, 2.0*sample.y - 1.0, 0.0);

        long double T = sqrt(1.0 - sample.y * sample.y);
        long double x = T * cos(sample.x);
        long double y = T * sin(sample.x);
        long double z = sample.y;

        mpvec3 point = mpvec3::normalize(mpvec3(x,y,z));

        VoronoiCell* b = new VoronoiCell(point);
        cells.push_back(b);
    }

    delete [] samples;

    // construct voronoi
    Voronoi voronoi;
    voronoi.construct(cells);

    // verify that each corner is closest to its origin point
    unsigned int corner_sum = 0;
    unsigned int incorrect = 0;
    unsigned int corner_count_correct = 0;
    for (auto it = cells.begin(); it != cells.end(); ++it)
    {
        VoronoiCell* b = *it;
        corner_sum += b->corners.size();

        for (auto ct = b->corners.begin(); ct != b->corners.end(); ++ct)
        {
            glm::dvec3 c = *ct;
            c += (b->position.to_dvec3() - c) * 0.1;

            long double iclose = glm::dot(b->position.to_dvec3(), c);

            bool correct = true;
            for (auto jt = cells.begin(); jt != cells.end(); ++jt)
            {
                if (jt != it)
                {
                    long double jclose = glm::dot((*jt)->position.to_dvec3(), c);
                    if (jclose > iclose)
                        correct = false;
                }
            }
            if (!correct)
                incorrect++;
        }

        if (b->corners.size() > 2)
            corner_count_correct++;
    }

    // assert correctness == 100%
    EXPECT_FLOAT_EQ( 1.0, ((long double)corner_sum-incorrect)/corner_sum );
    EXPECT_FLOAT_EQ( 1.0, ((long double)corner_count_correct)/cells.size() );
}

TEST(VoronoiTests, TestBeachLine)
{
    // generate VoronoiCell vector
    std::vector<VoronoiCell*> cells;

    const long double PI = M_PIl;

    int c = 40;
    MPSampleGenerator sample_generator;
    mpvec3* samples = sample_generator.getJitteredSamplesMP(c);

    for (int i = 0; i < c*c; i++)
    {
        mpvec3 sample = samples[i];

        // scale sample into range [0,2pi) x [-1,1]
        sample = mpvec3(2.0*PI*sample.x, 2.0*sample.y - 1.0, 0.0);

        long double T = sqrt(1.0 - sample.y * sample.y);
        long double x = T * cos(sample.x);
        long double y = T * sin(sample.x);
        long double z = sample.y;

        mpvec3 point = mpvec3::normalize(mpvec3(x,y,z));

        VoronoiCell* b = new VoronoiCell(point);
        cells.push_back(b);
    }

    delete [] samples;

    // construct voronoi
    Voronoi voronoi;
    voronoi.construct(cells);

    EXPECT_EQ( 2, voronoi.bl.getSize() );
}

TEST(VoronoiTests, TestCircumcenter)
{
    Voronoi vor;
    glm::dvec3 p1 = glm::normalize(glm::dvec3( 0.0, 1.0, 0.5f));
    glm::dvec3 p2 = glm::normalize(glm::dvec3( 1.0, 0.0, 0.5f));
    glm::dvec3 p3 = glm::normalize(glm::dvec3( 0.0,-1.0, 0.5f));

    glm::dvec3 cc = vor.circumcenter(p1,p2,p3).to_dvec3();

    EXPECT_FLOAT_EQ( 0.0, cc.x );
    EXPECT_FLOAT_EQ( 0.0, cc.y );
    EXPECT_FLOAT_EQ( 1.0, cc.z );
}

TEST(VoronoiTests, TestCircleBottom)
{
    Voronoi vor;

    glm::dvec3 cc = glm::dvec3(1.0,0.0,0.0);
    glm::dvec3 p  = glm::dvec3(0.0,1.0,0.0);
    long double cb = (long double)vor.circleBottom(cc, p);

    EXPECT_FLOAT_EQ( (long double)acos(-1.0), cb );
}
