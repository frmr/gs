#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <memory>
#include <string>
#include <vector>

#include <GL/gl3w.h>
#include "../matrix/Matrices.h"
#include "../voronoi/src/voronoi_generator.h"

#include "../cck/cck.h"
#include "gsArray.h"
#include "gsCamera.h"
#include "gsEdge.h"
#include "gsLandTile.h"
#include "gsMatrixStack.h"
#include "gsShader.h"
#include "gsTile.h"
#include "gsVertex.h"
#include "gsWaterTile.h"

using std::shared_ptr;
using std::string;
using std::vector;

namespace gs
{
    class Globe
    {
    private:
        GLuint              vao;

        //TODO: Make these const
        GLuint      positionVbo;
        GLuint      colorVbo;
        GLuint      texCoordVbo;
        GLuint      fogVbo;
        GLuint      indexBuffer;

        gs::Shader  shader;
        GLint       modelViewMatrixLocation;
        GLint       projectionMatrixLocation;
        GLint       normalMatrixLocation;
        int         numOfIndices;

        vector<shared_ptr<gs::Vertex>>      vertices;
        vector<shared_ptr<gs::Edge>>        edges;
        vector<shared_ptr<gs::Tile>>        allTiles;
        vector<shared_ptr<gs::LandTile>>    landTiles;
        vector<shared_ptr<gs::WaterTile>>   waterTiles;

    private:
        void                CombineVertices( const vector<glm::dvec3>& corners, gs::Array<vector<shared_ptr<gs::Vertex>>>& buckets, const unsigned int bucketDim, vector<shared_ptr<gs::Vertex>>& cellVertices );
        void                CreateTile( const vector<shared_ptr<gs::Vertex>>& cellVertices, const int vertexCount, const cck::Globe& terrain, const cck::GeoCoord& coord );
        void                CreateTileEdges( const vector<shared_ptr<gs::Vertex>>& cellVertices );
        //vector<gs::Tile>    ConstructTiles( const int quantity ) const;
        GLuint              CreateVbo( const void* data, const int size, const int components, const string& name );
        GLuint              CreateVbo( const int size, const int components, const string& name );
        void                GenerateBiomes( const int numOfSpreaders );
        int                 GenerateTiles( const int numOfTiles );
        static unsigned int HashDouble( const double val, const int bucketDim );
        static void         LinkTiles( const shared_ptr<gs::Tile> source, const shared_ptr<gs::Tile> dest, const shared_ptr<gs::Edge> edge );

    public:
        void        Draw( const gs::Camera& worldCamera ) const;
        cck::Globe  GenerateTerrain() const;
        void        PrintMeshProperties() const;
        void        Update();

    public:
        Globe();
    };
}

#endif // GS_GLOBE_H
