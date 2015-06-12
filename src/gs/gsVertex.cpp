#include "gsVertex.h"
#include "gsEdge.h"
#include "gsTile.h"

#include <iostream>

void gs::Vertex::AddEdge( const shared_ptr<gs::Edge> edge )
{
    edges.push_back( edge );
}

void gs::Vertex::AddTile( const shared_ptr<gs::Tile> tile )
{
    tiles.push_back( tile );
}

void gs::Vertex::CalculateHeight()
{
    double total = 0;
    for ( const auto& tile : tiles )
    {
        total += tile->GetHeight();
    }
    height = total / (double) tiles.size();
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
