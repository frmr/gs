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

bool gs::Edge::HasVertex( const shared_ptr<gs::Vertex>& refVertex ) const
{
    return ( v0->position == refVertex->position || v1->position == refVertex->position );
    //return ( v0 == refVertex || v1 == refVertex );
}

//bool gs::Edge::HasVertices( const gs::Vertex& refv0, const gs::Vertex& refv1 ) const
//{
//    return ( ( refv0.position == v0.position && refv1.position == v1.position ) || ( refv0.position == v1.position && refv1.position == v0.position ) );
//}

gs::Edge::Edge( const shared_ptr<gs::Vertex>& v0, const shared_ptr<gs::Vertex>& v1 )
    :   v0( v0 ),
        v1( v1 )
{
}
