#include "gsBiomeTextureGenerator.h"
#include "gsGlobe.h"
#include "gsLandTile.h"
#include "gsMath.h"
#include "gsRandomRange.h"
#include "gsTileGroupManager.h"

#include "../EasyBMP/EasyBMP.h"

#include <ctime>

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
    for ( unsigned int i = 0; i < texCoords.size(); ++i )
    {
        texCoords[i].x = ( (float) tileGroupTextureOffset.x + texCoords[i].x ) / (float) tileGroupTextureSize;
        texCoords[i].y = ( (float) tileGroupTextureOffset.y + texCoords[i].y ) / (float) tileGroupTextureSize;
    }
}

void gs::LandTile::DeleteLocalTextureData()
{
    texture.reset();
}

void gs::LandTile::GenerateTexture( const gs::BiomeTextureGenerator& biomeTextureGenerator )
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

    constexpr int pixelsPerUnit = 2000;

    int width = (int) ( maxCoord.x * pixelsPerUnit ) + 1;
    width = ( width < 1 ) ? 1 : width;
    int height = (int) ( maxCoord.y * pixelsPerUnit ) + 1;
    height = ( height < 1 ) ? 1 : height;

    texture = std::make_shared<gs::Texture>( width, height );

    vector<gs::Vec2i> pixelCoords;
    pixelCoords.reserve( vertices.size() );
    texCoords.reserve( vertices.size() );
    for ( const auto& coord : relativeCoords )
    {
        pixelCoords.emplace_back( (int) ( coord.x * pixelsPerUnit ), (int) ( coord.y * pixelsPerUnit ) ); //don't need all of them, just the first
        texCoords.emplace_back( (float) pixelCoords.back().x, (float) pixelCoords.back().y );
    }

    const float riverLimit = 0.002f;
    const float blendLimit = 0.004f;

    //add rivers
    //const gs::Vec3d xJump = refAxisU / (double) pixelsPerUnit;
    //const gs::Vec3d yJump = refAxisV / (double) pixelsPerUnit;

    //gs::Vec3d pixelOriginWorldCoord = (gs::Vec3d) vertices[0]->position - ( xJump * (double) pixelCoords[0].x ) - ( yJump * (double) pixelCoords[0].y ); //world coordinate of pixel (0,0)

    //create sublist of links to tiles that are rivers or need blending
    //vector<gs::Link<gs::LandTile>> notableLinks;
//    for ( const auto& link : landLinks )
//    {
//        if ( link.target->GetBiome() != biome || link.target->terrain != terrain || link.edge->IsRiver() )
//        {
//            notableLinks.push_back( link );
//        }
//    }

    //color each texel
    shared_ptr<const gs::Texture> sourceTexture = biomeTextureGenerator.GetTexture( biome, terrain );

    //texture->Blit( sourceTexture, gs::Vec2i() );

    for ( int x = 0; x < width; ++x )
    {
        const int sourceX = x % width;
        for ( int  y = 0; y < height; ++y )
        {
            const int sourceY = y % height;
            texture->SetColor( x, y, sourceTexture->GetRed( sourceX, sourceY ), sourceTexture->GetGreen( sourceX, sourceY ), sourceTexture->GetBlue( sourceX, sourceY ) );
        }
    }

    //for ( int x = 0; x < width; ++x )
    //{
        //for ( int y = 0; y < height; ++y )
        //{
//            gs::Vec3d pixelWorldCoord = pixelOriginWorldCoord + xJump * x + yJump * y; //TODO: Speed this up by using xJump and yJump to increment for each pixel
            //gs::Color texelColor = biomeTextureGenerator.Sample( pixelWorldCoord, biome, terrain );
//
//            //calculate distance to each notable edge
//            vector<float> notableDistances;
//            notableDistances.reserve( notableLinks.size() );
//            for ( const auto& link : notableLinks )
//            {
//                const gs::Vec3f closestEdgePoint = gs::ClosestPointOnLine( link.edge->v0->position, link.edge->vec, (gs::Vec3f) pixelWorldCoord, true );
//                notableDistances.push_back( ( closestEdgePoint - (gs::Vec3d) pixelWorldCoord ).Length() );
//            }
//
//            //TODO: Use iterators
//            for ( int i = 0; i < notableLinks.size(); ++i )
//            {
//                if ( notableLinks[i].edge->IsRiver() )
//                {
//                    if ( notableDistances[i] < riverLimit )
//                    {
//                        texelColor = gs::Color( 0, 0, 255 );
//                        break;
//                    }
//                }
//                else if ( notableLinks[i].target->GetBiome() != biome || notableLinks[i].target->terrain != terrain )
//                {
//                    if ( notableDistances[i] < blendLimit )
//                    {
//                        //blend if there are no closer tiles of the same biome
//                        bool foundCloser = false;
//                        for ( int j = 0; j < notableLinks.size(); ++j )
//                        {
//                            if ( i == j )
//                            {
//                                continue;
//                            }
//                            if ( notableLinks[i].target->GetBiome() == notableLinks[j].target->GetBiome() )
//                            {
//                                if ( notableDistances[j] < notableDistances[i] )
//                                {
//                                    foundCloser = true;
//                                    break;
//                                }
//                            }
//                        }
//
//                        //blend
//                        if ( !foundCloser )
//                        {
//                            const gs::Color neighborColor = biomeTextureGenerator.Sample( pixelWorldCoord, notableLinks[i].target->GetBiome(), notableLinks[i].target->terrain );
//                            const gs::Color colorDifference = neighborColor - texelColor;
//                            const double blendFactor = ( 1.0f - ( notableDistances[i] / blendLimit ) ) * 0.5f;
//                            texelColor += gs::Color( colorDifference.x * blendFactor, colorDifference.y * blendFactor, colorDifference.z * blendFactor );
//                        }
//                    }
//                }
//            }
            //texture->SetColor( x, y, texelColor );
        //}
    //}
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

    if ( rand.Sample() < probability )
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
        texture( nullptr )
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
}

gs::LandTile::~LandTile()
{
    DeleteLocalTextureData();
}
