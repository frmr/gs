#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <memory>
#include <string>
#include <vector>

#include "../gl3w/gl3w.h"
#include "../matrix/Matrices.h"
#include "../voronoi/src/voronoi_generator.h"

#include "../cck/cck.h"
#include "gsArray.h"
#include "gsCamera.h"
#include "gsEdge.h"
#include "gsLandTile.h"
#include "gsLandTileBuffer.h"
#include "gsMatrixStack.h"
#include "gsShader.h"
#include "gsTile.h"
#include "gsTileGroupManager.h"
#include "gsVertex.h"
#include "gsWaterTile.h"
#include "gsWaterTileBuffer.h"

using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::vector;

namespace gs
{
    class Globe
    {
    private:
        GLuint              vao;

        shared_ptr<gs::LandTileBuffer>  landBuffer; //TODO: unique_ptr
        shared_ptr<gs::WaterTileBuffer> waterBuffer;

        gs::Shader  shader;
        GLint       modelViewMatrixLocation;
        GLint       projectionMatrixLocation;
        GLint       normalMatrixLocation;
        int         numOfIndices;

        vector<gs::VertexPtr>       vertices;
        vector<gs::EdgePtr>         edges;
        vector<gs::TilePtr>         allTiles;
        vector<gs::LandTilePtr>     landTiles;
        vector<gs::WaterTilePtr>    waterTiles;

        vector<gs::LandTile::Biome> biomeTable; //TODO: Would be better as a gs::Array

        gs::TileGroupManager        groupManager;

    private:
        void                    AssignBufferOffsetsToTiles();
        void                    BuildBiomeTable();
        void                    CombineVertices( const vector<glm::dvec3>& corners, gs::Array<vector<gs::VertexPtr>>& buckets, const unsigned int bucketDim, vector<gs::VertexPtr>& cellVertices );
        void                    CreateTile( const vector<gs::VertexPtr>& cellVertices, const cck::Globe& terrain, const cck::Vec3& centroid );
        void                    CreateTileEdges( const vector<gs::VertexPtr>& cellVertices );
        //GLuint                  CreateVbo( const void* data, const int size, const int components, const string& name );
        GLuint                  CreateVbo( const int size, const int components, const string& name );
        void                    GenerateBiomes( const int numOfSpreaders );
        void                    GenerateRivers( const int numOfSpawners );
        int                     GenerateTiles( const int numOfTiles );
        static unsigned int     HashDouble( const double val, const int bucketDim );
        static void             LinkTiles( const gs::TilePtr source, const gs::TilePtr dest, const gs::EdgePtr edge );
        gs::LandTile::Biome     LookupRegionBiome( const int id ) const;
        void                    SetTileGroupTextureSize();

    public:
        void                    Draw( const gs::Camera& worldCamera ) const;
        cck::Globe              GenerateTerrain() const;
        void                    PrintMeshProperties() const;
        void                    Update();

    public:
        Globe();
    };
}

#endif // GS_GLOBE_H
