#include "../src/priqueue.h"
#include "../src/voronoi_event.h"
#include "../src/mpreal/mpvec3.h"

TEST(PriQueueTests, TestInsertLinkedList)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    // do checks
    EXPECT_EQ(ce1->pnode->prev , (PriQueueNode*)NULL);
    EXPECT_EQ(ce1->pnode->next , ce2->pnode);

    EXPECT_EQ(ce2->pnode->prev , ce1->pnode);
    EXPECT_EQ(ce2->pnode->next , ce3->pnode);

    EXPECT_EQ(ce3->pnode->prev , ce2->pnode);
    EXPECT_EQ(ce3->pnode->next , ce4->pnode);

    EXPECT_EQ(ce4->pnode->prev , ce3->pnode);
    EXPECT_EQ(ce4->pnode->next , ce5->pnode);

    EXPECT_EQ(ce5->pnode->prev , ce4->pnode);
    EXPECT_EQ(ce5->pnode->next , (PriQueueNode*)NULL);

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestInsertOutOfOrderLinkedList)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    // do checks
    EXPECT_EQ(ce1->pnode->prev , (PriQueueNode*)NULL);
    EXPECT_EQ(ce1->pnode->next , ce2->pnode);

    EXPECT_EQ(ce2->pnode->prev , ce1->pnode);
    EXPECT_EQ(ce2->pnode->next , ce3->pnode);

    EXPECT_EQ(ce3->pnode->prev , ce2->pnode);
    EXPECT_EQ(ce3->pnode->next , ce4->pnode);

    EXPECT_EQ(ce4->pnode->prev , ce3->pnode);
    EXPECT_EQ(ce4->pnode->next , ce5->pnode);

    EXPECT_EQ(ce5->pnode->prev , ce4->pnode);
    EXPECT_EQ(ce5->pnode->next , (PriQueueNode*)NULL);

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestInsertReverseOrderLinkedList)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    // do checks
    EXPECT_EQ(ce1->pnode->prev , (PriQueueNode*)NULL);
    EXPECT_EQ(ce1->pnode->next , ce2->pnode);

    EXPECT_EQ(ce2->pnode->prev , ce1->pnode);
    EXPECT_EQ(ce2->pnode->next , ce3->pnode);

    EXPECT_EQ(ce3->pnode->prev , ce2->pnode);
    EXPECT_EQ(ce3->pnode->next , ce4->pnode);

    EXPECT_EQ(ce4->pnode->prev , ce3->pnode);
    EXPECT_EQ(ce4->pnode->next , ce5->pnode);

    EXPECT_EQ(ce5->pnode->prev , ce4->pnode);
    EXPECT_EQ(ce5->pnode->next , (PriQueueNode*)NULL);

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestInsertSkips)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    // do checks
    PriQueueNode* curr = pq.head;

    while (curr != NULL)
    {
        for (int i = 0; i < 12; i++)
        {
            if (curr->skips[i] != NULL) EXPECT_EQ( curr, curr->skips[i]->prev_skips[i] );
            if (curr->next != NULL) EXPECT_EQ( curr, curr->next->prev );
        }

        curr = curr->next;
    }

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestEraseSkips)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    pq.erase(ce3->pnode);

    // do checks
    PriQueueNode* curr = pq.head;

    while (curr != NULL)
    {
        for (int i = 0; i < 12; i++)
        {
            if (curr->skips[i] != NULL) EXPECT_EQ( curr, curr->skips[i]->prev_skips[i] );
            if (curr->next != NULL) EXPECT_EQ( curr, curr->next->prev );
        }

        curr = curr->next;
    }

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestEraseHeadSkips)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    pq.erase(ce1->pnode);

    // do checks
    PriQueueNode* curr = pq.head;

    while (curr != NULL)
    {
        for (int i = 0; i < 12; i++)
        {
            if (curr->skips[i] != NULL) EXPECT_EQ( curr, curr->skips[i]->prev_skips[i] );
            if (curr->next != NULL) EXPECT_EQ( curr, curr->next->prev );
        }

        curr = curr->next;
    }

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestEraseTailSkips)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    pq.erase(ce5->pnode);

    // do checks
    PriQueueNode* curr = pq.head;

    while (curr != NULL)
    {
        for (int i = 0; i < 12; i++)
        {
            if (curr->skips[i] != NULL) EXPECT_EQ( curr, curr->skips[i]->prev_skips[i] );
            if (curr->next != NULL) EXPECT_EQ( curr, curr->next->prev );
        }

        curr = curr->next;
    }

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
}

TEST(PriQueueTests, TestErasePopPushSkips)
{
    // initialize
    PriQueue pq;

    CircleEvent* ce3 = new CircleEvent(3.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce3);

    CircleEvent* ce1 = new CircleEvent(1.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce1);

    CircleEvent* ce5 = new CircleEvent(5.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce5);

    CircleEvent* ce4 = new CircleEvent(4.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce4);

    CircleEvent* ce2 = new CircleEvent(2.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce2);

    pq.erase(ce5->pnode);
    pq.pop();
    pq.erase(ce3->pnode);

    CircleEvent* ce6 = new CircleEvent(2.5, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce6);

    CircleEvent* ce7 = new CircleEvent(7.0, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce7);

    pq.pop();
    pq.pop();
    pq.pop();

    CircleEvent* ce8 = new CircleEvent(1.1, 0.0, mpvec3(0.0,0.0,0.0), NULL);
    pq.push(ce8);

    // do checks
    PriQueueNode* curr = pq.head;

    while (curr != NULL)
    {
        for (int i = 0; i < 12; i++)
        {
            if (curr->skips[i] != NULL) EXPECT_EQ( curr, curr->skips[i]->prev_skips[i] );
            if (curr->next != NULL) EXPECT_EQ( curr, curr->next->prev );
        }

        curr = curr->next;
    }

    // cleanup
    delete ce1;
    delete ce2;
    delete ce3;
    delete ce4;
    delete ce5;
    delete ce6;
    delete ce7;
    delete ce8;
}