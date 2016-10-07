#pragma once

#include "gsEdge.hpp"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class Edge;

    template<typename T>
    class Link
    {
    public:
        const shared_ptr<T>         target;
        const shared_ptr<gs::Edge>  edge;

    public:
        Link(const shared_ptr<T> target, const shared_ptr<gs::Edge> edge)
            :   target(target),
                edge(edge)
        {
        }
    };
}