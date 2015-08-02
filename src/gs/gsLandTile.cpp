#include "gsGlobe.h"
#include "gsLandTile.h"
#include "gsMath.h"
#include "gsRandomRange.h"
#include "gsTileGroupManager.h"

#include "../EasyBMP/EasyBMP.h"

#include <ctime>

gs::RandomRange<int> gs::LandTile::colorGenerator = gs::RandomRange<int>( 0, 255, 0 );

gs::LandTile::Terrain gs::LandTile::DetermineTerrain() const
{
    if ( height < 0.25 )
    {
        return gs::LandTile::Terrain::PLAINS;
    }
    else if ( height < 0.7 )
    {
        return gs::LandTile::Terrain::HILLS;
    }
    else
    {
        return gs::LandTile::Terrain::MOUNTAINS;
    }
}

void gs::LandTile::AddToTileGroupTexture( shared_ptr<gs::Texture> tileGroupTexture, const gs::Vec2i& tileGroupTextureOffset, const int tileGroupTextureSize )
{
    tileGroupTexture->Blit( texture, tileGroupTextureOffset );
    //update texture coordinates to be relative to the texture group texture
    for ( int i = 0; i < texCoords.size(); ++i )
    {
        texCoords[i].x = ( (float) tileGroupTextureOffset.x + texCoords[i].x ) / (float) tileGroupTextureSize;
        texCoords[i].y = ( (float) tileGroupTextureOffset.y + texCoords[i].y ) / (float) tileGroupTextureSize;
    }
}

void gs::LandTile::DeleteLocalTextureData()
{
    texture.reset();
}

void gs::LandTile::GenerateTexture()
{
    //TODO: Make this safer by checking for presence of first and second vertices

    //reference u-axis is from v0 to v1
    const gs::Vec3d refAxisU = (gs::Vec3d) ( vertices[1]->position - vertices[0]->position ).Unit();
    //reference v-axis is the cross-product of u-axis and the tile normal
    const gs::Vec3d refAxisV = gs::Cross<double>( refAxisU, normal ).Unit(); //TODO: Second argument might actually be worldVertices[0]->position

    vector<gs::Vec2d> relativeCoords;
    relativeCoords.reserve( vertices.size() );

    //use reference axes to compute relative coordinates of each world vertex
    for ( const auto& vert : vertices )
    {
        relativeCoords.emplace_back( gs::Dot<double>( refAxisU, (gs::Vec3d) ( vert->position - vertices[0]->position ) ),
                                     gs::Dot<double>( refAxisV, (gs::Vec3d) ( vert->position - vertices[0]->position ) ) );
    }

    //compute bounding box
    gs::Vec2d minCoord( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
    gs::Vec2d maxCoord( std::numeric_limits<double>::min(), std::numeric_limits<double>::min() );
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
    minCoord -= minCoord; //should always be (0,0)

    constexpr int pixelsPerUnit = 1000;

    int width = (int) ( maxCoord.x * pixelsPerUnit ) + 1;
    width = ( width < 1 ) ? 1 : width;
    int height = (int) ( maxCoord.y * pixelsPerUnit ) + 1;
    height = ( height < 1 ) ? 1 : height;

    texture = std::make_shared<gs::Texture>( width, height );



    //fill with random color
    GLubyte randRed = (GLubyte) colorGenerator.Sample();
    GLubyte randBlue = (GLubyte) colorGenerator.Sample();
    GLubyte randGreen = (GLubyte) colorGenerator.Sample();
    for ( int x = 0; x < width; ++x )
    {
        for ( int y = 0; y < height; ++y )
        {
            texture->SetColor( x, y, randRed, randGreen, randBlue );
        }
    }



    vector<gs::Vec2i> pixelCoords;
    pixelCoords.reserve( vertices.size() );
    for ( const auto& coord : relativeCoords )
    {
        pixelCoords.emplace_back( (int) ( coord.x * pixelsPerUnit ), (int) ( coord.y * pixelsPerUnit ) );
        texCoords.emplace_back( (float) pixelCoords.back().x, (float) pixelCoords.back().y );
    }

    float riverLimit = 1.0f / (float) pixelsPerUnit * 2.0f;

    //add rivers
    const gs::Vec3d xJump = refAxisU / (double) pixelsPerUnit;
    const gs::Vec3d yJump = refAxisV / (double) pixelsPerUnit;

    gs::Vec3d pixelOriginWorldCoord = (gs::Vec3d) vertices[0]->position - ( xJump * (double) pixelCoords[0].x ) - ( yJump * (double) pixelCoords[0].y ); //world coordinate of pixel (0,0)

    for ( int x = 0; x < width; ++x )
    {
        for ( int y = 0; y < height; ++y )
        {
            gs::Vec3d pixelWorldCoord = pixelOriginWorldCoord + xJump * x + yJump * y; //TODO: Speed this up by using xJump and yJump to increment for each pixel
            for ( const auto& link : landLinks )
            {
                if ( link.edge->IsRiver() )
                {
                    const gs::Vec3f closestRiverPoint = gs::ClosestPointOnLine( link.edge->v0->position, link.edge->vec, (gs::Vec3f) pixelWorldCoord, true );
                    const float riverDist = ( closestRiverPoint - (gs::Vec3d) pixelWorldCoord ).Length();
                    if ( riverDist < riverLimit )
                    {
                        texture->SetColor( x, y, 0, 0, 255 );
                    }
                }
            }
        }
    }

    //mark vertices on texture
    for ( const auto& coord : pixelCoords )
    {
        texture->SetRed( coord.x, coord.y, 255 );
        texture->SetGreen( coord.x, coord.y, 0 );
        texture->SetBlue( coord.x, coord.y, 0 );
    }
}

shared_ptr<gs::Texture> gs::LandTile::GetTexture() const
{
    return texture;
}

vector<shared_ptr<gs::LandTile>> gs::LandTile::GetUnassignedBiomeNeighbors() const
{
    vector<shared_ptr<gs::LandTile>> unassignedNeighbors;
    for ( const auto& link : landLinks )
    {
        if ( link.target->GetBiome() == gs::LandTile::Biome::UNASSIGNED )
        {
            unassignedNeighbors.push_back( link.target );
        }
    }
    return unassignedNeighbors;
}

gs::LandTile::Biome gs::LandTile::GetBiome() const
{
    return biome;
}

bool gs::LandTile::HasUnassignedBiomeNeighbors() const
{
    for ( const auto& link : landLinks )
    {
        if ( link.target->GetBiome() == gs::LandTile::Biome::UNASSIGNED )
        {
            return true;
        }
    }
    return false;
}

void gs::LandTile::SetBiome( const gs::LandTile::Biome newBiome )
{
    biome = newBiome;
}

void gs::LandTile::SetBlackIfRiver() //TODO: delete
{
    if ( biome == gs::LandTile::Biome::DESERT )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 232.0/255.0, 243.0/255.0, 10.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 200.0/255.0, 208.0/255.0, 9.0/255.0 );
        else
            color = gs::Vec3f( 137.0/255.0, 143.0/255.0, 5.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::GRASSLAND )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 85.0/255.0, 222.0/255.0, 61.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 55.0/255.0, 185.0/255.0, 32.0/255.0 );
        else
            color = gs::Vec3f( 35.0/255.0, 116.0/255.0, 20.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::SEMI_ARID )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 236.0/255.0, 162.0/255.0, 23.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 183.0/255.0, 124.0/255.0, 15.0/255.0 );
        else
            color = gs::Vec3f( 128.0/255.0, 87.0/255.0, 11.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::TUNDRA )
    {
        if ( terrain == gs::LandTile::Terrain::PLAINS )
            color = gs::Vec3f( 200.0/255.0, 200.0/255.0, 200.0/255.0 );
        else if ( terrain == gs::LandTile::Terrain::HILLS )
            color = gs::Vec3f( 150.0/255.0, 150.0/255.0, 150.0/255.0 );
        else
            color = gs::Vec3f( 100.0/255.0, 100.0/255.0, 100.0/255.0 );
    }
    else if ( biome == gs::LandTile::Biome::UNASSIGNED )
    {
        color = gs::Vec3f( 1.0f, 1.0f, 1.0f );
    }

    for ( auto link : landLinks )
    {
        if ( link.edge->IsRiver() )
        {
            color = gs::Vec3f( 0.0, 0.0, 0.0 );
        }
    }
}

bool gs::LandTile::SpawnRiver( const int newRiverId, gs::RandomRange<double>& rand )
{
    double probability = 0.0;
    if ( terrain == gs::LandTile::Terrain::PLAINS )
    {
        probability = 0.05;
    }
    else if ( terrain == gs::LandTile::Terrain::HILLS )
    {
        probability = 0.5;
    }
    else if ( terrain == gs::LandTile::Terrain::MOUNTAINS )
    {
        probability = 1.0;
    }

    if ( rand.Sample() > probability )
    {
        vertices[(int) ( rand.Sample() * (double) vertices.size() )]->SetRiver( newRiverId );
        return true;
    }
    else
    {
        return false;
    }
}

void gs::LandTile::UpdateAllBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo, const GLuint texCoordVbo ) //const
{
    UpdatePositionBuffer( positionVbo );
    UpdateColorBuffer( colorVbo );
    UpdateFogBuffer( fogVbo );
    UpdateTexCoordBuffer( texCoordVbo );
}

void gs::LandTile::UpdateTexCoordBuffer( const GLuint texCoordVbo )
{
    GLfloat* texCoordArray = new GLfloat[2*vertices.size()];
    for ( unsigned int i = 0; i < texCoords.size(); ++i )
    {
        texCoordArray[2*i] = texCoords[i].x;
        texCoordArray[2*i+1] = texCoords[i].y;
    }
    glBindBuffer( GL_ARRAY_BUFFER, texCoordVbo );
    glBufferSubData( GL_ARRAY_BUFFER, 2 * bufferOffset * sizeof(GLfloat), 2 * vertices.size() * sizeof(GLfloat), texCoordArray );
    delete[] texCoordArray;
}

gs::LandTile::LandTile( const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height, const int regionId )
    :   gs::Tile( gs::Tile::Type::LAND, vertices, centroid, height ),
        regionId( regionId ),
        terrain( DetermineTerrain() ),
        forested( false ),
        biome( gs::LandTile::Biome::UNASSIGNED ),
        texture( NULL )
{
    if ( terrain == gs::LandTile::Terrain::PLAINS )
    {
        color = gs::Vec3f( 0.0f, 0.8f, 0.0f );
    }
    else if ( terrain == gs::LandTile::Terrain::HILLS )
    {
        color = gs::Vec3f( 0.0f, 0.4f, 0.0f );
    }
    else if ( terrain == gs::LandTile::Terrain::MOUNTAINS )
    {
        color = gs::Vec3f( 0.4, 0.4, 0.4 );
    }
    //color = gs::Vec3f( 0.0f, ( 8.0f * (float) id ) / 255.0f, 0.0f );
}

gs::LandTile::~LandTile()
{
    DeleteLocalTextureData();
}
