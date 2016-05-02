#include "priqueue.h"

#include <cstring>
#include <iostream>
#include <algorithm>

PriQueueNode::PriQueueNode(CircleEvent* ce)
{
    memset(skips, 0, sizeof(PriQueueNode*) * 20);
    next = NULL;
    this->ce = ce;
    ce->pnode = this;

    memset(prev_skips, 0, sizeof(PriQueueNode*) * 20);
    prev = NULL;
}

PriQueue::PriQueue()
{
    head = NULL;
}

PriQueue::~PriQueue()
{
    while (head != NULL)
    {
        PriQueueNode* d = head;
        head = head->next;
        delete d;
    }
}

// 25.01% - 32.13%
void PriQueue::push(CircleEvent* ce)
{
    PriQueueNode* node = new PriQueueNode(ce);

    if (head == NULL)
    {
        head = node;
    }
    else
    {
        if (comp(head->ce, ce)) // insert at front
        {
            node->next = head;
            head->prev = node;
            for (int i = 0; i < 20; i++)
            {
                if (head->skips[i] == NULL) break;
                node->skips[i] = head->skips[i];
                head->skips[i]->prev_skips[i] = node;
                head->skips[i] = NULL;
            }
            head = node;
            return;
        }

        int skip_level = 19;
        PriQueueNode* nodes[20];
        PriQueueNode* curr = head;

        bool done = false;
        while (!done)
        {
            if (curr->skips[skip_level] != NULL && comp(ce , curr->skips[skip_level]->ce))
            {
                curr = curr->skips[skip_level];
            }
            else
            {
                nodes[skip_level--] = curr;
                if (skip_level < 0) done = true;
            }
        }

        // continue search on the linked list level
        while (curr->next != NULL && comp(ce , curr->next->ce))
        {
            curr = curr->next;
        }

        // insert after curr
        node->next = curr->next;
        node->prev = curr;
        if (curr->next != NULL) curr->next->prev = node;
        curr->next = node;

        addSkips(node, nodes);
    }
}

void PriQueue::addSkips(PriQueueNode* node, PriQueueNode** previous)
{
    int skip_count = 20 - log2(std::max((int)(distribution(generator) * 2097152), 1));

    for(int i = 0; i < skip_count; i++)
    {
        node->skips[i] = (*previous)->skips[i];
        if (node->skips[i] != NULL) node->skips[i]->prev_skips[i] = node;

        node->prev_skips[i] = (*previous);
        (*previous)->skips[i] = node;

        previous++;
    }
}

CircleEvent* PriQueue::top()
{
    if (head == NULL)
        return NULL;
    else
        return head->ce;
}

void PriQueue::pop()
{
    if (head == NULL) return;

    if (head->next != NULL)
    {
        for (int i = 0; i < 20; i++)
        {
            if (head->skips[i] == NULL) break;

            if (head->skips[i] != head->next)
            {
                head->next->skips[i] = head->skips[i];
                head->skips[i]->prev_skips[i] = head->next;
            }
            head->next->prev_skips[i] = NULL;
        }

        head->next->prev = NULL;

        PriQueueNode* d = head;
        head = head->next;
        delete d;
    }
    else
    {
        delete head;
        head = NULL;
    }
}

bool PriQueue::empty()
{
    return head == NULL;
}


void PriQueue::erase(PriQueueNode* node)
{
    if (node->prev == NULL)
    {
        pop();
    }
    else if (node->next == NULL)
    {
        node->prev->next = NULL;

        for (int i = 0; i < 20; i++)
        {
            if (node->prev_skips[i] == NULL) break;
            node->prev_skips[i]->skips[i] = NULL;
        }

        delete node;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;

        for (int i = 0; i < 20; i++)
        {
            bool c = false;

            if (node->skips[i] != NULL)
            {
                node->skips[i]->prev_skips[i] = node->prev_skips[i];
                c = true;
            }

            if (node->prev_skips[i] != NULL)
            {
                node->prev_skips[i]->skips[i] = node->skips[i];
                c = true;
            }

            if (!c) break;
        }

        delete node;
    }
}