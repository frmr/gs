#include "gsTileTexture.h"
#include "gsMath.h"

#include <iostream>
#include <string>
using std::cerr;
using std::endl;

void gs::TileTexture::DeleteTextureData()
{
    //delete image;
}

int gs::TileTexture::GetArea() const
{
    return width * height;
}

int gs::TileTexture::GetHeight() const
{
    return height;
}

int gs::TileTexture::GetWidth() const
{
    return width;
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

    width = (int) ( maxCoord.x * pixelsPerUnit ) + 1;
    width = ( width < 1 ) ? 1 : width;
    height = (int) ( maxCoord.y * pixelsPerUnit ) + 1;
    height = ( height < 1 ) ? 1 : height;

    image = new BMP();
    image->SetSize( width, height );
    for ( const auto& coord : relativeCoords )
    {
        int x = (int) ( coord.x * pixelsPerUnit );
        //x = ( x < 0 ) ? 0 : x;
        int y = (int) ( coord.y * pixelsPerUnit );
        //y = ( y < 0 ) ? 0 : y;
        //cerr << width << " " << height << " " << x << " " << y << endl;
        (*image)(x,y)->Red = 255;
        (*image)(x,y)->Green = 0;
        (*image)(x,y)->Blue = 0;
    }
    image->WriteToFile( ( "data/textures/procedural/" + std::to_string( id ) + ".bmp" ).c_str() );
}

gs::TileTexture::~TileTexture()
{
    DeleteTextureData();
}
