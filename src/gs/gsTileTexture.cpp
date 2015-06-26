#include "gsTileTexture.h"
#include "gsMath.h"

#include <iostream>
#include <string>
using std::cerr;
using std::endl;

gs::Vec2f gs::TileTexture::ComputeRelativePoint( const gs::Vec3f& uAxis, const gs::Vec3f& vAxis, const gs::Vec3f& p )
{
//    gs::Vec3f pVec = point - vert0;
//
//    const float dotUU = VectorDot( uAxis, uAxis );
//    const float dotUV = VectorDot( uAxis, vAxis );
//    const float dotUP = VectorDot( uAxis, p );
//    const float dot22 = VectorDot( vec02, vec02 );
//    const float dot2p = VectorDot( vec02, pVec );
//
//    const float invDenom = 1.0f / ( dot11 * dot22 - dot12 * dot12 );
//    const float u = ( dot22 * dot1p - dot12 * dot2p ) * invDenom;
//    const float v = ( dot11 * dot2p - dot12 * dot1p ) * invDenom;
}

gs::TileTexture::TileTexture( const int id, const vector<gs::VertexPtr>& worldVertices, const vector<gs::Link<gs::Tile>>& links, const gs::Vec3f& centroid )
{
    //TODO: Make this safer by checking for presence of first and second vertices
    gs::Vec3f normal = centroid;
    normal.Unit();

    //reference u-axis is from v0 to v1
    gs::Vec3f refAxisU = ( worldVertices[1]->position - worldVertices[0]->position ).Unit();
    //reference v-axis is the cross-product of u-axis and the tile normal
    gs::Vec3f refAxisV = gs::Cross<float>( refAxisU, normal ).Unit();

    vector<gs::Vec2f> relativeCoords;
    relativeCoords.reserve( worldVertices.size() );

    //use reference axes to compute relative coordinates of each world vertex
    for ( const auto& vert : worldVertices )
    {
        //relativeCoords.push_back( ( vert->position - worldVertices[0]->position ).ToVec2() );
        relativeCoords.push_back( gs::Vec2f( gs::Dot<float>( refAxisU, vert->position - worldVertices[0]->position ),
                                             gs::Dot<float>( refAxisV, vert->position - worldVertices[0]->position ) ) );
    }

    //compute bounding box
    gs::Vec2f minCoord = gs::Vec2f( std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
    gs::Vec2f maxCoord = gs::Vec2f( std::numeric_limits<float>::min(), std::numeric_limits<float>::min() );
    for ( const auto& coord : relativeCoords )
    {
        if ( coord.x < minCoord.x ) { minCoord.x = coord.x; }
        if ( coord.y < minCoord.y ) { minCoord.y = coord.y; }
        if ( coord.x > maxCoord.x ) { maxCoord.x = coord.x; }
        if ( coord.y > maxCoord.y ) { maxCoord.y = coord.y; }
    }

    //shift coordinates by bounding box minimum
    for ( auto& coord : relativeCoords )
    {
        coord -= minCoord;
    }
    maxCoord -= minCoord;
    minCoord -= minCoord; //should be (0,0)

    constexpr int pixelsPerUnit = 1000;

    file.SetSize( (int) ( maxCoord.x * pixelsPerUnit ), (int) ( maxCoord.y * pixelsPerUnit ) );
    file.WriteToFile( ( "data/textures/procedural/" + std::to_string( id ) + ".bmp" ).c_str() );
}
