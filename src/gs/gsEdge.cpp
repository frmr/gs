#include "gsEdge.h"

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

bool gs::Edge::HasVertices( const gs::Vec3f& refv0, const gs::Vec3f& refv1 ) const
{
    return ( ( refv0 == v0 && refv1 == v1 ) || ( refv0 == v1 && refv1 == v0 ) );
}

gs::Edge::Edge( const gs::Vec3f& v0, const gs::Vec3f& v1 )
    :   v0( v0 ),
        v1( v1 )
{
}
