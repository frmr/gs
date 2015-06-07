#ifndef GS_LINK_H
#define GS_LINK_H

#include "gsEdge.h"

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
        Link( const shared_ptr<T> target, const shared_ptr<gs::Edge> edge )
            :   target( target ),
                edge( edge )
        {
        }

    };
}

#endif // GS_LINK_H
