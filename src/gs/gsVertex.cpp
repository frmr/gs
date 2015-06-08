#include "gsVertex.h"
#include "gsEdge.h"

void gs::Vertex::AddEdge( const shared_ptr<gs::Edge> edge )
{
    edges.push_back( edge );
}

vector<shared_ptr<gs::Edge>> gs::Vertex::GetEdges() const
{
    return edges;
}

shared_ptr<gs::Edge> gs::Vertex::GetEdgeWith( const shared_ptr<gs::Vertex> refVertex ) const
{
    for ( auto edge : edges )
    {
        if ( edge->HasVertex( refVertex ) )
        {
            return edge;
        }
    }
    return nullptr;
}

gs::Vertex::Vertex( const gs::Vec3d& position )
    :   position( position )
{
}
