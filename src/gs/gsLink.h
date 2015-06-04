#ifndef GS_LINK_H
#define GS_LINK_H

#include "gsEdge.h"
#include "gsTile.h"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class Edge;
    class Tile;

    class Link
    {
    public:
        const shared_ptr<gs::Tile>  target;
        const shared_ptr<gs::Edge>  edge;
    public:
        Link( const shared_ptr<gs::Tile> target, const shared_ptr<gs::Edge> edge )
            :   target( target ),
                edge( edge )
        {
        }

    };
}

#endif // GS_LINK_H
