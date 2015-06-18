#include "gsVertex.h"
#include "gsEdge.h"
#include "gsTile.h"

#include <algorithm>
#include <iostream>
#include <limits>

using std::cerr;
using std::endl;

int gs::Vertex::idCounter = 0;

void gs::Vertex::AddLink( const gs::Link<gs::Vertex> link )
{
    links.push_back( link );
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
    for ( auto link : links )
    {
        if ( link.edge->HasVertex( refVertex ) )
        {
            return link.edge;
        }
    }
    return nullptr;
}

double gs::Vertex::GetHeight() const
{
    return height;
}

bool gs::Vertex::IsRiver() const
{
    return river;
}

void gs::Vertex::SetRiver()
{
    vector<int> riverIds;
    SetRiver( riverIds );
}

void gs::Vertex::SetRiver( vector<int>& riverIds )
{
    if ( river )
    {
        return;
    }

    river = true;
    riverIds.push_back( id );

    //stop if vertex touches the sea
    bool coastal = false;
    for ( const auto& tile : tiles )
    {
        if ( tile->GetSurface() == gs::Tile::Type::WATER )
        {
            coastal = true;
            break;
        }
    }

    if ( coastal )
    {
        return;
    }

    gs::EdgePtr lowestEdge = nullptr;
    gs::VertexPtr lowestVertex = nullptr;
    double lowestHeight = std::numeric_limits<double>::max();

    for ( auto& link : links )
    {
        bool targetVisited = ( std::find( riverIds.begin(), riverIds.end(), link.target->id ) != riverIds.end() );
        if ( !targetVisited && link.target->GetHeight() < lowestHeight )
        {
            lowestEdge = link.edge;
            lowestVertex = link.target;
            lowestHeight = link.target->GetHeight();
        }
    }

    lowestEdge->SetRiver();
    lowestVertex->SetRiver( riverIds );
}

gs::Vertex::Vertex( const gs::Vec3d& position )
    :   id( idCounter++ ),
        position( position ),
        river( false )
{
}
