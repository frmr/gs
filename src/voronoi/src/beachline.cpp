#include "beachline.h"

#include <cstring>
#include <algorithm>
#include <iostream>


// SkipNode Implementation //

SkipNode::SkipNode(SiteEvent* s)
{
    memset(skips, 0, sizeof(SkipNode*) * 8);
    memset(p_skips, 0, sizeof(SkipNode*) * 8);
    prev = next = NULL;
    site = s;
    sweepline_pos = 0.0;
    range_end = 0.0;
}

SkipNode::~SkipNode()
{
    delete site;
}

long double SkipNode::getRangeEnd(long double sweepline, long double shift)
{
    if (sweepline > sweepline_pos)
    {
        range_end = intersect(site, next->site, sweepline, shift);
        sweepline_pos = sweepline;
    }

    return range_end;
}

constexpr long double PI2 = M_PIl*2.0;
constexpr long double PI2i = 0.5/M_PIl;

// 16.25% - 10.46%
long double SkipNode::intersect(SiteEvent* sa, SiteEvent* sb, long double sweepline, long double shift)
{
    long double eps = (cos(sa->polar) - cos(sb->polar)) * sin(sweepline);

    long double cos_eps = cos(sweepline);

    long double a = (cos_eps - cos(sb->polar)) * sin(sa->polar) * cos(sa->azimuth) -
              (cos_eps - cos(sa->polar)) * sin(sb->polar) * cos(sb->azimuth);

    long double b = (cos_eps - cos(sb->polar)) * sin(sa->polar) * sin(sa->azimuth) -
              (cos_eps - cos(sa->polar)) * sin(sb->polar) * sin(sb->azimuth);

    long double ab_hyp = sqrt(a*a + b*b);

    long double gamma;
    if (b > 0) gamma = asin(a/ab_hyp);
    else
    {
        if (a > 0) gamma = acos(b/ab_hyp);
        else gamma = acos(b/ab_hyp) - 2*asin(a/ab_hyp);
    }

    long double azi = asin(eps / ab_hyp) - gamma;

    azi += shift;
    azi = azi * PI2i;
    azi = azi - floor(azi);

    return azi * PI2;
}


// BeachLine Implementation //

BeachLine::BeachLine()
{
    size = 0;
    linked_list = NULL;
    distribution = std::uniform_real_distribution<double>(0.0,1.0);
}

BeachLine::~BeachLine()
{
    while (size > 0)
    {
        SkipNode* next = linked_list->next;
        delete linked_list;
        linked_list = next;
        size--;
    }
}

int BeachLine::getSize()
{
    return size;
}

SkipNode* BeachLine::insert(SiteEvent* se, long double sweepline)
{
    // create a new SkipNode
    SkipNode* node = new SkipNode(se);

    // find where it should be inserted
    findAndInsert(node, sweepline);
    return node;
}

void BeachLine::erase(SkipNode* node)
{
    // change starting position for searches if necessary
    if (node == linked_list)
    {
        if (linked_list->skips[7] != linked_list)
        {
            linked_list = linked_list->skips[7];
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                if (linked_list->next->skips[i] == NULL)
                {
                    linked_list->next->skips[i] = linked_list->skips[i];
                    linked_list->skips[i]->p_skips[i] = linked_list->next;
                    linked_list->next->p_skips[i] = linked_list;
                    linked_list->skips[i] = linked_list->next;
                }
            }

            linked_list = linked_list->next;
        }
    }

    removeSkips(node);

    // erase from list
    node->prev->next = node->next;
    node->next->prev = node->prev;

    delete node;
    size--;
}

void BeachLine::removeSkips(SkipNode* node)
{
    for (int i = 0; i < 8; i++)
    {
        if (node->skips[i] == NULL) break;

        node->skips[i]->p_skips[i] = node->p_skips[i];
        node->p_skips[i]->skips[i] = node->skips[i];
    }
}

// 3.75% - 6.72%
void BeachLine::findAndInsert(SkipNode* node, long double sweepline)
{
    // shift positions on beachline such that the new insertion point goes to zero
    // this means we want to search for the element with the largest post intersection value

    if (size == 0)
    {
        insertAfter(node, NULL);
        return;
    }
    if (size == 1)
    {
        // insert after existing node
        insertAfter(node, linked_list);
        addSkips(node,&linked_list,true);
        return;
    }

    long double shift = 2.0 * M_PIl - node->site->azimuth;

    int skip_level = 7;
    SkipNode* nodes[8];
    SkipNode* curr = linked_list;

    bool done = false;
    while (!done)
    {
        if (curr->skips[skip_level]->getRangeEnd(sweepline, shift) >
            curr->getRangeEnd(sweepline, shift))
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
    while (curr->next->getRangeEnd(sweepline, shift) > curr->getRangeEnd(sweepline, shift))
    {
        curr = curr->next;
    }

    curr = curr->next;

    // split node and insert in between
    SkipNode* copy = new SkipNode(new SiteEvent(curr->site));

    insertAfter(node, curr);
    insertAfter(copy, node);

    addSkips(node,nodes,false);
    addSkips(copy,nodes,false);
}

void BeachLine::insertAfter(SkipNode* node, SkipNode* at)
{
    if (at == NULL)
    {
        node->next = node;
        node->prev = node;
        linked_list = node;

        for (int i = 0; i < 8; i++)
        {
            node->skips[i] = node;
            node->p_skips[i] = node;
        }
    }
    else
    {
        // insert into list
        SkipNode* next = at->next;
        at->next = node;
        node->prev = at;
        node->next = next;
        next->prev = node;
    }

    size++;
}

int log2(int n)
{
    const unsigned int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    const unsigned int S[] = {1, 2, 4, 8, 16};

    register unsigned int r = 0; // result of log2(v) will go here
    for (int i = 3; i >= 0; i--) // unroll for speed...
    {
      if (n & b[i])
      {
        n >>= S[i];
        r |= S[i];
      }
    }

    return r;
}

void BeachLine::addSkips(SkipNode* node, SkipNode** previous, bool repeat_first)
{
    int skip_count = 8 - log2(std::max((int)(distribution(generator) * 512), 1));

    for(int i = 0; i < skip_count; i++)
    {
        node->skips[i] = (*previous)->skips[i];
        node->skips[i]->p_skips[i] = node;
        (*previous)->skips[i] = node;
        node->p_skips[i] = (*previous);

        if (!repeat_first)
        {
            (*previous) = node;
            previous++;
        }
    }
}
