#include "gsGlobe.h"

#include <iostream>

#include "../voronoi/src/voronoi_generator.h"
#include "gsRandomRange.h"
#include "../cck/cck.h"

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

    terrain.LinkNodes( 1,     2,  -1.6,   4.0,    150.0,  50.0 );     //France, Iberia
    terrain.LinkNodes( 1,     3,  -0.3,   0.75,   150.0,  50.0 );     //France, Germany
    terrain.LinkNodes( 1,     4,  -0.3,   5.0,    150.0,  50.0 );     //France, Italy
    terrain.LinkNodes( 4,     2,  -0.3,   0.75,   150.0,  50.0 );     //Italy, Iberia
    terrain.LinkNodes( 3,     4,  -0.3,   5.0,    150.0,  50.0 );     //Germany, Italy

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

gs::Globe::Globe()
    :   shader( "test", "data/shaders/test.vert", "data/shaders/test.frag" )
{
    //generate world
    cck::Globe terrain = GenerateTerrain();

    //generate voronoi sphere
    int numOfTiles = 10000;
    VoronoiGenerator vg;
    vg.generateTessellation( numOfTiles );

    //count vertices and initialise tiles
    int vertexCount = 0;
    tiles.reserve( numOfTiles );

    for ( const auto& cell : vg.cell_vector )
    {
        vector<gs::Vec3f> cellVertices;
        for ( const auto& corner : cell->corners )
        {
            cellVertices.push_back( gs::Vec3f( corner.x, corner.y, corner.z ) );
        }

        tiles.push_back( std::make_shared<gs::Tile>( vertexCount, cellVertices, terrain ) );
        vertexCount += cell->corners.size();

        //create edge and link to other tile if initialised
        for ( unsigned int i = 0; i < cellVertices.size(); ++i )
        {
            gs::Vec3f v0 = cellVertices[i];
            gs::Vec3f v1 = cellVertices[( i == cellVertices.size() - 1 ) ? 0 : i + 1];

            bool foundEdge = false;
            for ( const auto& edge : edges )
            {
                if ( edge->HasVertices( v0, v1 ) )
                {
                    edge->AddTile( tiles.back() );

                    //link tiles of each side of the edge to eachother
                    vector<shared_ptr<gs::Tile>> edgeTiles = edge->GetTiles();
                    edgeTiles.front()->AddLink( gs::Link( edgeTiles.back(), edge ) );
                    edgeTiles.back()->AddLink( gs::Link( edgeTiles.front(), edge ) );

                    foundEdge = true;
                    break;
                }
            }

            if ( !foundEdge )
            {
                edges.push_back( std::make_shared<gs::Edge>( v0, v1 ) );
                edges.back()->AddTile( tiles.back() );
            }
        }
    }

    //create vao
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    //copy data to vram
    positionVbo = CreateVbo( vertexCount, 3, "positionVert" );
    colorVbo = CreateVbo( vertexCount, 3, "colorVert");
    texCoordVbo = CreateVbo( vertexCount, 2, "texCoordVert" );
    fogVbo = CreateVbo( vertexCount, 1, "fogVert" );

    vector<GLuint> indexVector;

    for ( auto& tile : tiles )
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

    shader.Link();

    modelViewMatrixLocation = shader.GetUniformLocation( "modelViewMatrix" );
    projectionMatrixLocation = shader.GetUniformLocation( "projectionMatrix" );
    normalMatrixLocation = shader.GetUniformLocation( "normalMatrix" );
}
