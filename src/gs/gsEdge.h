#ifndef GS_EDGE_H
#define GS_EDGE_H

#include <memory>
#include <vector>

#include "gsVec3.h"

using std::shared_ptr;
using std::vector;

namespace gs
{
    class Tile;
    class Vertex;

    class Edge
    {
    private:
        vector<shared_ptr<gs::Tile>>    tiles;
        bool                            river;

    public:
        const shared_ptr<gs::Vertex>    v0;
        const shared_ptr<gs::Vertex>    v1;
        const gs::Vec3f                 vec;

    public:
        bool                            AddTile( const shared_ptr<gs::Tile>& newTile );
        bool                            BordersTile( const shared_ptr<gs::Tile>& refTile ) const;
        vector<shared_ptr<gs::Tile>>    GetTiles() const;
        bool                            HasVertex( const shared_ptr<gs::Vertex>& refVertex ) const;
        //bool                            HasVertices( const gs::Vertex& refv0, const gs::Vertex& refv1 ) const;
        bool                            IsRiver() const;
        void                            SetRiver();

    public:
        Edge( const shared_ptr<gs::Vertex>& v0, const shared_ptr<gs::Vertex>& v1 );
    };

    typedef shared_ptr<gs::Edge> EdgePtr;
}

#endif // GS_EDGE_H
