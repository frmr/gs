#include "gsGlobe.h"

#include <iostream>

#include "../voronoi/src/voronoi_generator.h"

#include "gsBinarySearchTree.h"
#include "gsRandomRange.h"
#include "gsSpreader.h"

using std::cerr;
using std::endl;

void gs::Globe::Draw( const gs::Camera& worldCamera ) const
{
    shader.Use();

    glDisable( GL_CULL_FACE );

    //get the inverse model view matrix
    Matrix4 inverseModelViewMatrix = worldCamera.GetViewMatrix();
    inverseModelViewMatrix.invert();

    //pass matrices to shaders
    glUniformMatrix4fv( modelViewMatrixLocation, 1, false, worldCamera.GetViewMatrix().get() ); //TODO: multiply by model matrix if needed
    glUniformMatrix4fv( projectionMatrixLocation, 1, false, worldCamera.GetProjectionMatrix().get() );
    glUniformMatrix4fv( normalMatrixLocation, 1, false, inverseModelViewMatrix.getTranspose() );

    //draw
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glDrawElements( GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0 );
}

cck::Globe gs::Globe::GenerateTerrain() const
{
    cck::Globe terrain( 6370.0, 0 ); //TODO: random seed
    terrain.SetNoiseParameters( 8, 0.75, 0.00015 );

    terrain.AddNode( 0,   52.0,   -4.0,   -0.2,   0.5,    600.0 );    //Britain
    terrain.AddNode( 1,   46.0,   2.0,    -0.2,   0.5,    600.0 );    //France
    terrain.AddNode( 2,   40.0,   -4.0,   -0.2,   1.0,    750.0 );    //Iberia
    terrain.AddNode( 3,   50.0,   10.0,   -0.3,   0.75,   500.0 );    //Germany
    terrain.AddNode( 4,   42.0,   15.0,   -0.3,   0.75,   550.0 );    //Italy
    terrain.AddNode( 5,   65.0,   17.0,   -1.0,   1.5,    1000.0 );   //Scandinavia
    terrain.AddNode( 6,   50.0,   25.0,   -0.2,   0.5,    800.0 );    //Poland
    terrain.AddNode( 7,   40.0,   25.0,   -0.5,   1.5,    600.0 );    //Balkans
    terrain.AddNode( 8,   57.0,   44.0,   -0.3,   0.75,   1200.0 );   //Russia
    terrain.AddNode( 9,   38.0,   38.0,   -1.5,   3.0,    1000.0 );   //Turkey
    terrain.AddNode( 10,  26.0,   44.0,   -0.5,   1.0,    1600.0 );   //Arabia
    terrain.AddNode( 11,  35.0,   56.0,   -1.0,   2.0,    700.0 );    //Persia
    terrain.AddNode( 12,  48.0,   68.0,   -0.2,   0.75,   1100.0 );   //Central Asia
    terrain.AddNode( 13,  65.0,   75.0,   -0.2,   0.5,    1300.0 );   //Siberia 1
    terrain.AddNode( 14,  65.0,   106.0,  -0.3,   0.75,   1400.0 );   //Siberia 2
    terrain.AddNode( 15,  32.0,   88.0,   1.0,    4.0,    500.0 );    //Himalayas
    terrain.AddNode( 29,  44.0,   95.0,   -1.0,   2.5,    800.0 );    //Xinjiang
    terrain.AddNode( 16,  25.0,   80.0,   -0.4,   1.0,    1000.0 );   //North India
    terrain.AddNode( 17,  15.0,   77.0,   -0.4,   1.0,    1000.0 );   //South India
    terrain.AddNode( 18,  25.0,   102.0,  -1.0,   2.0,    700.0 );    //Burma
    terrain.AddNode( 19,  14.0,   103.0,  -0.5,   0.75,   800.0 );    //Indochina
    terrain.AddNode( 20,  0.0,    103.0,  -0.4,   1.0,    850.0 );    //Malaysia
    terrain.AddNode( 21,  30.0,   107.0,  -0.5,   2.0,    400.0 );    //Inner China
    terrain.AddNode( 22,  23.0,   115.0,  -1.0,   1.5,    600.0 );    //South China
    terrain.AddNode( 23,  35.0,   115.0,  -0.3,   0.5,    400.0 );    //North China
    terrain.AddNode( 24,  47.0,   109.0,  -0.5,   1.5,    900.0 );    //Mongolia
    terrain.AddNode( 25,  38.0,   127.0,  -0.5,   0.75,   350.0 );    //Korea
    terrain.AddNode( 26,  65.0,   144.0,  -0.5,   1.0,    800.0 );    //Siberia 3
    terrain.AddNode( 27,  62.0,   165.0,  -1.0,   2.0,    1000.0 );   //Siberia 4
    terrain.AddNode( 28,  36.0,   138.0,  -1.0,   1.5,    800.0 );    //Japan

    terrain.LinkNodes( 1,     2,  -1.6,   4.0,    150.0,  50.0 );     //France, Iberia
    terrain.LinkNodes( 1,     3,  -0.3,   0.75,   150.0,  50.0 );     //France, Germany
    terrain.LinkNodes( 1,     4,  -0.3,   5.0,    150.0,  50.0 );     //France, Italy
    terrain.LinkNodes( 4,     2,  -0.3,   0.75,   150.0,  50.0 );     //Italy, Iberia
    terrain.LinkNodes( 4,     7,  -0.5,   0.75,   150.0,  50.0 );     //Italy, Balkans
    terrain.LinkNodes( 3,     4,  -0.3,   5.0,    150.0,  50.0 );     //Germany, Italy
    terrain.LinkNodes( 3,     5,  -0.75,  0.75,   150.0,  50.0 );     //Germany, Scandinavia
    terrain.LinkNodes( 3,     6,  -0.4,   0.5,    150.0,  50.0 );     //Germany, Poland
    terrain.LinkNodes( 3,     7,  -0.75,  1.5,    150.0,  50.0 );     //Germany, Balkans
    terrain.LinkNodes( 5,     8,  -1.0,   1.5,    150.0,  50.0 );     //Scandinavia, Russia
    terrain.LinkNodes( 5,     6,  -1.0,   1.5,    150.0,  50.0 );     //Scandinavia, Poland
    terrain.LinkNodes( 6,     8,  -0.5,   0.5,    150.0,  50.0 );     //Poland, Russia
    terrain.LinkNodes( 6,     7,  -1.0,   1.0,    150.0,  50.0 );     //Poland, Balkans
    terrain.LinkNodes( 7,     9,  -3.0,   3.0,    150.0,  50.0 );     //Balkans, Turkey
    terrain.LinkNodes( 8,     9,  -1.5,   4.0,    150.0,  50.0 );     //Russia, Turkey
    terrain.LinkNodes( 7,     8,  -1.0,   3.0,    150.0,  50.0 );     //Balkans, Russia
    terrain.LinkNodes( 9,     10, -3.0,   3.0,    150.0,  50.0 );     //Turkey, Arabia
    terrain.LinkNodes( 8,     12, -0.5,   0.75,   150.0,  50.0 );     //Russia, Central Asia
    terrain.LinkNodes( 8,     13, -0.3,   0.75,   150.0,  50.0 );     //Russia, Siberia 1
    terrain.LinkNodes( 12,    13, -0.25,  0.75,   150.0,  50.0 );     //Central Asia, Siberia 1
    terrain.LinkNodes( 9,     12, -0.2,   0.75,   150.0,  50.0 );     //Turkey, Central Asia
    terrain.LinkNodes( 9,     11, -3.0,   3.0,    150.0,  50.0 );     //Turkey, Persia
    terrain.LinkNodes( 11,    12, -2.0,   2.0,    150.0,  50.0 );     //Persia, Central Asia
    terrain.LinkNodes( 13,    14, -0.5,   0.75,   150.0,  50.0 );     //Siberia 1, Siberia 2
    terrain.LinkNodes( 11,    16, -2.0,   2.0,    150.0,  50.0 );     //Persia, North India
    terrain.LinkNodes( 16,    17, -0.5,   1.0,    150.0,  50.0 );     //North India, South India
    terrain.LinkNodes( 16,    15, -0.5,   1.0,    150.0,  50.0 );     //North India, Himalayas
    terrain.LinkNodes( 12,    16, -0.5,   1.0,    150.0,  50.0 );     //Central Asia, North India
    terrain.LinkNodes( 12,    15, -0.2,   0.75,   150.0,  50.0 );     //Central Asia, Himalayas
    terrain.LinkNodes( 16,    18, -0.5,   1.0,    150.0,  50.0 );     //North India, Burma
    terrain.LinkNodes( 18,    19, -1.0,   1.5,    150.0,  50.0 );     //Burma, Indochina
    terrain.LinkNodes( 19,    20, -0.5,   0.75,   150.0,  50.0 );     //Indochina, Malaysia
    terrain.LinkNodes( 15,    18, -1.0,   1.5,    150.0,  50.0 );     //Himalayas, Burma
    terrain.LinkNodes( 18,    21, -1.0,   1.5,    150.0,  50.0 );     //Burma, Inner China
    terrain.LinkNodes( 18,    22, -1.0,   1.5,    150.0,  50.0 );     //Burma, South China
    terrain.LinkNodes( 21,    22, -0.5,   2.0,    150.0,  50.0 );     //Inner China, South China
    terrain.LinkNodes( 21,    15, 1.0,    2.0,    150.0,  50.0 );     //Inner China, Himalayas
    terrain.LinkNodes( 22,    23, -0.5,   1.0,    150.0,  50.0 );     //South China, North China
    terrain.LinkNodes( 21,    23, -0.5,   2.0,    150.0,  50.0 );     //Inner China, North China
    terrain.LinkNodes( 23,    24, -0.3,   0.5,    150.0,  50.0 );     //North China, Mongolia
    terrain.LinkNodes( 21,    24, -0.5,   1.5,    150.0,  50.0 );     //Inner China, Mongolia
    terrain.LinkNodes( 14,    24, -0.5,   1.5,    150.0,  50.0 );     //Siberia 2, Mongolia
    terrain.LinkNodes( 14,    26, -0.5,   1.0,    150.0,  50.0 );     //Siberia 2, Siberia 3
    terrain.LinkNodes( 24,    26, -0.5,   1.0,    150.0,  50.0 );     //Mongolia, Siberia 3
    terrain.LinkNodes( 24,    25, -0.5,   1.5,    150.0,  50.0 );     //Mongolia, Korea
    terrain.LinkNodes( 25,    26, -0.5,   1.0,    150.0,  50.0 );     //Korea, Siberia 3
    terrain.LinkNodes( 26,    27, -0.5,   1.0,    150.0,  50.0 );     //Siberia 3, Siberia 4

    terrain.LinkNodes( 12,    29, -0.2,   0.75,   150.0,  50.0 );     //Central Asia, Xinjiang
    terrain.LinkNodes( 13,    29, -0.2,   0.5,    150.0,  50.0 );     //Siberia 1, Xinjiang
    terrain.LinkNodes( 14,    29, -0.3,   0.75,   150.0,  50.0 );     //Siberia 2, Xinjiang
    terrain.LinkNodes( 24,    29, -0.5,   1.5,    150.0,  50.0 );     //Mongolia, Xinjiang
    terrain.LinkNodes( 15,    29, 0.0,    2.5,    150.0,  50.0 );     //Himalayas, Xinjiang
    terrain.LinkNodes( 21,    29, -0.5,   2.0,    150.0,  50.0 );     //Inner, Xinjiang

    return terrain;
}

void gs::Globe::Update()
{

}

GLuint gs::Globe::CreateVbo( const void* data, const int size, const int components, const string& name )
{
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLuint location = shader.GetAttribLocation( name );

    glBufferData( GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW );
    glVertexAttribPointer( location, components, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( location );

    return vbo;
}

GLuint gs::Globe::CreateVbo( const int elements, const int components, const string& name )
{
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLuint location = shader.GetAttribLocation( name );

    glBufferData( GL_ARRAY_BUFFER, elements * components * sizeof(GLfloat), NULL, GL_STATIC_DRAW );
    glVertexAttribPointer( location, components, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( location );

    return vbo;
}

//val must be between 0 and 1
unsigned int gs::Globe::HashDouble( const double val, const int buckets )
{
    int hashVal = ( ( (unsigned int) ( val / 1.0 * (double) buckets ) ) + buckets ) / 2;
    if ( hashVal > buckets - 1 )
    {
        hashVal = buckets - 1;
    }
    else if ( hashVal < 0 )
    {
        hashVal = 0;
    }
    return (unsigned int) hashVal;
}

int gs::Globe::GenerateTiles( const int numOfTiles )
{
    VoronoiGenerator vg;
    vg.generateTessellation( numOfTiles );

    //generate world
    cck::Globe terrain = GenerateTerrain();

    //count vertices and initialise tiles
    int vertexCount = 0;
    allTiles.reserve( numOfTiles );

    //join identical vertices
    constexpr unsigned int bucketDim = 256;
    double errorMargin = 0.000001;
    auto* buckets = new vector<shared_ptr<gs::Vertex>>[bucketDim][bucketDim][bucketDim];

    for ( const auto& cell : vg.cell_vector )
    {
        vector<shared_ptr<gs::Vertex>> cellVertices;
        for ( const auto& corner : cell->corners )
        {
            const gs::Vec3d gsCorner( corner.x, corner.y, corner.z );
            //hash to buckets
            unsigned int xHash = HashDouble( gsCorner.x, bucketDim );
            unsigned int yHash = HashDouble( gsCorner.y, bucketDim );
            unsigned int zHash = HashDouble( gsCorner.z, bucketDim );

            bool foundVertex = false;

            for ( unsigned int xi = ( ( xHash == 0 ) ? xHash : xHash - 1 ); xi <= ( ( xHash == bucketDim - 1 ) ? xHash : xHash + 1 ); ++xi )
            {
                for ( unsigned int yi = ( ( yHash == 0 ) ? yHash : yHash - 1 ); yi <= ( ( yHash == bucketDim - 1 ) ? yHash : yHash + 1 ); ++yi )
                {
                    for ( unsigned int zi = ( ( zHash == 0 ) ? zHash : zHash - 1 ); zi <= ( ( zHash == bucketDim - 1 ) ? zHash : zHash + 1 ); ++zi )
                    {
                        for ( auto v : buckets[xi][yi][zi] )
                        {
                            if ( v->position.x < gsCorner.x + errorMargin && v->position.x > gsCorner.x - errorMargin &&
                                 v->position.y < gsCorner.y + errorMargin && v->position.y > gsCorner.y - errorMargin &&
                                 v->position.z < gsCorner.z + errorMargin && v->position.z > gsCorner.z - errorMargin )
                            {
                                cellVertices.push_back( v );
                                foundVertex = true;
                                break;
                            }
                        }
                    }
                    if ( foundVertex ) { break; }
                }
                if ( foundVertex ) { break; }
            }

            if ( !foundVertex )
            {
                auto newVertex = std::make_shared<gs::Vertex>( gsCorner );
                cellVertices.push_back( newVertex );
                vertices.push_back( newVertex );
                buckets[xHash][yHash][zHash].push_back( newVertex );
            }
        }

        //create new tile
        double sampleHeight;
        int sampleId;
        terrain.SampleData( cck::Vec3( cell->centroid.z, cell->centroid.x, cell->centroid.y ).ToGeographic(), sampleHeight, sampleId );

        if ( sampleHeight > 0.00001 )
        {
            auto newTile = std::make_shared<gs::LandTile>( vertexCount, cellVertices, sampleHeight, sampleId );
            allTiles.push_back( std::dynamic_pointer_cast<gs::Tile>( newTile ) );
            landTiles.push_back( newTile );
        }
        else
        {
            auto newTile = std::make_shared<gs::WaterTile>( vertexCount, cellVertices );
            allTiles.push_back( std::dynamic_pointer_cast<gs::Tile>( newTile ) );
            waterTiles.push_back( newTile );
        }

        vertexCount += cellVertices.size();

        //create edges
        for ( unsigned int i = 0; i < cellVertices.size(); ++i )
        {
            auto v0 = cellVertices[i];
            auto v1 = cellVertices[( i == cellVertices.size() - 1 ) ? 0 : i + 1];

            auto edge = v0->GetEdgeWith( v1 );

            if ( edge == nullptr )
            {
                auto newEdge = std::make_shared<gs::Edge>( v0, v1 );
                newEdge->AddTile( allTiles.back() );
                v0->AddEdge( newEdge );
                v1->AddEdge( newEdge );
                edges.push_back( newEdge );
            }
            else
            {
                edge->AddTile( allTiles.back() );

                //link tiles on each side of the edge to each other
                vector<shared_ptr<gs::Tile>> edgeTiles = edge->GetTiles();

                LinkTiles( edgeTiles.front(), edgeTiles.back(), edge );
                LinkTiles( edgeTiles.back(), edgeTiles.front(), edge );
            }
        }

    }

    delete[] buckets;

    cerr << "Vertices: " << vertices.size() << endl;
    cerr << "Edges: " << edges.size() << endl;

    for ( auto edge : edges )
    {
        if ( edge->GetTiles().size() != 2 )
        {
            cerr << "gs::Globe::GenerateTiles() in gsGlobe.cpp: Edge has " << edge->GetTiles().size() << " adjacent face." << endl;
        }
    }

    return vertexCount;
}

void gs::Globe::LinkTiles( const shared_ptr<gs::Tile> source, const shared_ptr<gs::Tile> dest, const shared_ptr<gs::Edge> edge )
{
    source->AddLink( gs::Link<gs::Tile>( dest, edge ) );
    if ( dest->GetSurface() == gs::Tile::Type::LAND )
    {
        source->AddLink( gs::Link<gs::LandTile>( std::dynamic_pointer_cast<gs::LandTile>( dest ), edge ) );
    }
    else if ( dest->GetSurface() == gs::Tile::Type::WATER )
    {
        source->AddLink( gs::Link<gs::WaterTile>( std::dynamic_pointer_cast<gs::WaterTile>( dest ), edge ) );
    }
    else
    {
        cerr << "gs::Globe::GenerateTiles() in gsGlobe.cpp: Face adjacent to edge has unassigned type." << endl;
    }
}

gs::Globe::Globe()
    :   shader( "test", "data/shaders/test.vert", "data/shaders/test.frag" )
{
    //generate voronoi sphere
    const int numOfTiles = 16000;
    const int numOfVertices = GenerateTiles( numOfTiles );

    //create vao
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    //copy data to vram
    positionVbo = CreateVbo( numOfVertices, 3, "positionVert" );
    colorVbo = CreateVbo( numOfVertices, 3, "colorVert");
    texCoordVbo = CreateVbo( numOfVertices, 2, "texCoordVert" );
    fogVbo = CreateVbo( numOfVertices, 1, "fogVert" );

    vector<GLuint> indexVector;

    for ( auto& tile : allTiles )
    {
        tile->InitBuffers( positionVbo, colorVbo, texCoordVbo, fogVbo, indexVector );
    }

    GLuint* indexArray = new GLuint[indexVector.size()];
    for ( unsigned int i = 0; i < indexVector.size(); ++i )
    {
        indexArray[i] = indexVector[i];
    }

    numOfIndices = indexVector.size();

    //indices
    glGenBuffers( 1, &indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW );

    delete[] indexArray;

    shader.SetFragOutput( "colorOut" );

    shader.Link();

    modelViewMatrixLocation = shader.GetUniformLocation( "modelViewMatrix" );
    projectionMatrixLocation = shader.GetUniformLocation( "projectionMatrix" );
    normalMatrixLocation = shader.GetUniformLocation( "normalMatrix" );
}
