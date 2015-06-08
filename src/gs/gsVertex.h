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

    class Vertex
    {
    public:
        const gs::Vec3d position;

    private:
        vector<shared_ptr<gs::Edge>> edges;

    public:
        void                            AddEdge( const shared_ptr<gs::Edge> edge );
        vector<shared_ptr<gs::Edge>>    GetEdges() const;
        shared_ptr<gs::Edge>            GetEdgeWith( const shared_ptr<gs::Vertex> refVertex ) const;

    public:
        Vertex( const gs::Vec3d& position );
    };
}

#endif // GS_VERTEX_H
