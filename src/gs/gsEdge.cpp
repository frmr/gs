#include "gsEdge.h"
#include "gsTile.h"
#include "gsVertex.h"

bool gs::Edge::AddTile( const shared_ptr<gs::Tile>& newTile )
{
    if ( tiles.size() == 2 )
    {
        return false;
    }
    tiles.push_back( newTile );
    return true;
}

bool gs::Edge::BordersTile( const shared_ptr<gs::Tile>& refTile ) const
{
    for ( const auto& tile : tiles )
    {
        if ( tile == refTile )
        {
            return true;
        }
    }
    return false;
}

vector<shared_ptr<gs::Tile>> gs::Edge::GetTiles() const
{
    return tiles;
}

bool gs::Edge::HasVertex( const shared_ptr<gs::Vertex>& refVertex ) const
{
    return ( v0->position == refVertex->position || v1->position == refVertex->position );
}

bool gs::Edge::IsRiver() const
{
    return river;
}

void gs::Edge::SetRiver()
{
    river = true;
}

gs::Edge::Edge( const shared_ptr<gs::Vertex>& v0, const shared_ptr<gs::Vertex>& v1 )
    :   river( false ),
        v0( v0 ),
        v1( v1 ),
        vec( v1->position - v0->position )
{
}
