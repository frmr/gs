#ifndef GS_VERTEX_H
#define GS_VERTEX_H

#include <memory>
#include <vector>

#include "gsVec3.h"

using std::shared_ptr;
using std::vector;

namespace gs
{
    class Edge;
    class Tile;

    class Vertex
    {
    public:
        const gs::Vec3d position;

    private:
        vector<shared_ptr<gs::Edge>>    edges;
        vector<shared_ptr<gs::Tile>>    tiles;

        double                          height;

    public:
        void                            AddEdge( const shared_ptr<gs::Edge> edge );
        void                            AddTile( const shared_ptr<gs::Tile> tile );
        void                            CalculateHeight();
        vector<shared_ptr<gs::Edge>>    GetEdges() const;
        shared_ptr<gs::Edge>            GetEdgeWith( const shared_ptr<gs::Vertex> refVertex ) const;

    public:
        Vertex( const gs::Vec3d& position );
    };

    typedef shared_ptr<gs::Vertex> VertexPtr;
}

#endif // GS_VERTEX_H
