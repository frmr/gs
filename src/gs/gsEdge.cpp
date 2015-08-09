#include "gsEdge.h"
#include "gsTile.h"
#include "gsVertex.h"

int gs::Edge::idCounter = 0;

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
    return ( v0->GetPosition() == refVertex->GetPosition() || v1->GetPosition() == refVertex->GetPosition() );
}

bool gs::Edge::IsRiver() const
{
    return river;
}

void gs::Edge::SetRiver()
{
    river = true;
}

void gs::Edge::Widen()
{
    const float length = (float) vec.Length();
    if ( length < 0.01f )
    {
        vec.Unit();
        v1->SetPosition( v1->GetPosition() + vec * ( 0.01f - length ) / 2.0f );
        v0->SetPosition( v0->GetPosition() - vec * ( 0.01f - length ) / 2.0f );
        vec = v1->GetPosition() - v0->GetPosition();
    }
}

gs::Edge::Edge( const shared_ptr<gs::Vertex>& v0, const shared_ptr<gs::Vertex>& v1 )
    :   river( false ),
        id( idCounter++ ),
        v0( v0 ),
        v1( v1 ),
        vec( v1->GetPosition() - v0->GetPosition() )
{
}
