#ifndef BEACHLINE_H
#define BEACHLINE_H

#include "voronoi_event.h"
#include <random>

class SiteEvent;

class SkipNode
{
    public:

        SkipNode(SiteEvent* s);
        ~SkipNode();
        long double getRangeEnd(long double sweepline, long double shift);
        long double intersect(SiteEvent* sa, SiteEvent* sb, long double sweepline, long double shift);

        SkipNode* skips[8];
        SkipNode* p_skips[8];
        SkipNode* prev;
        SkipNode* next;

        SiteEvent* site;
        long double sweepline_pos;
        long double range_end;
};

/* 
    This class manages the beachline for voronoi tessellation.
*/

class BeachLine
{
    public:

        BeachLine();
        ~BeachLine();

        int getSize();

        SkipNode* insert(SiteEvent* se, long double sweepline);
        void erase(SkipNode* node);

    private:

        SkipNode* linked_list;

        int size;

        void findAndInsert(SkipNode* node, long double sweepline);
        void insertAfter(SkipNode* node, SkipNode* at);

        void addSkips(SkipNode* node, SkipNode** previous, bool repeat_first);
        void removeSkips(SkipNode* node);

        // for randomly determining the number of skip levels to add
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution;
};

#endif