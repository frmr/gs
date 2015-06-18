#ifndef GS_VERTEX_H
#define GS_VERTEX_H

#include <memory>
#include <vector>

#include "gsLink.h"
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
        const int       id;
        const gs::Vec3d position;

    private:
        static int idCounter;

        vector<shared_ptr<gs::Edge>>    edges;
        vector<shared_ptr<gs::Tile>>    tiles;
        vector<gs::Link<gs::Vertex>>    links;

        double                          height;
        bool                            river;

    private:
        bool                            CompareTargetHeights( const gs::Link<gs::Vertex>& a, const gs::Link<gs::Vertex>& b ) const;

    public:
        void                            AddLink( const gs::Link<gs::Vertex> link );
        void                            AddTile( const shared_ptr<gs::Tile> tile );
        void                            CalculateHeight();
        vector<shared_ptr<gs::Edge>>    GetEdges() const;
        shared_ptr<gs::Edge>            GetEdgeWith( const shared_ptr<gs::Vertex> refVertex ) const;
        double                          GetHeight() const;
        bool                            IsRiver() const;
        void                            SetRiver();
        void                            SetRiver( vector<int>& riverIds );
        void                            SortLinksByHeight();

    public:
        Vertex( const gs::Vec3d& position );
    };

    typedef shared_ptr<gs::Vertex> VertexPtr;
}

#endif // GS_VERTEX_H
