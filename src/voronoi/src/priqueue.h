#ifndef PRIQUEUE_H
#define PRIQUEUE_H

#include "voronoi_event.h"
#include "voronoi_event_compare.h"
#include <random>

//#include "gtest/gtest_prod.h"

class PriQueueNode
{
    public:

        PriQueueNode(CircleEvent* ce);

        PriQueueNode* skips[20];
        PriQueueNode* next;

        PriQueueNode* prev_skips[20];
        PriQueueNode* prev;

        CircleEvent* ce;
};

class PriQueue
{
    public:

        PriQueue();
        ~PriQueue();

        void push(CircleEvent* ce);
        CircleEvent* top();
        void pop();
        bool empty();

        void erase(PriQueueNode* node);

    private:

        PriQueueNode* head;
        VoronoiEventCompare comp;

        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution;

        void addSkips(PriQueueNode* node, PriQueueNode** previous);

        // tests
//        FRIEND_TEST(PriQueueTests, TestInsertLinkedList);
//        FRIEND_TEST(PriQueueTests, TestInsertOutOfOrderLinkedList);
//        FRIEND_TEST(PriQueueTests, TestInsertReverseOrderLinkedList);
//        FRIEND_TEST(PriQueueTests, TestInsertSkips);
//        FRIEND_TEST(PriQueueTests, TestEraseSkips);
//        FRIEND_TEST(PriQueueTests, TestEraseHeadSkips);
//        FRIEND_TEST(PriQueueTests, TestEraseTailSkips);
//        FRIEND_TEST(PriQueueTests, TestErasePopPushSkips);
};

#endif
