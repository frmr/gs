#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <memory>
#include <string>
#include <vector>

#include <GL/gl3w.h>
#include "../matrix/Matrices.h"

#include "gsCamera.h"
#include "gsEdge.h"
#include "gsMatrixStack.h"
#include "gsShader.h"
#include "gsTile.h"

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

        //vector<shared_ptr<gs::Vec3>>    vertices;
        vector<shared_ptr<gs::Tile>>    tiles;
        vector<shared_ptr<gs::Edge>>    edges;

    private:
        vector<gs::Tile>    ConstructTiles( const int quantity ) const;
        GLuint              CreateVbo( const void* data, const int size, const int components, const string& name );
        GLuint              CreateVbo( const int size, const int components, const string& name );
        int                 GenerateTiles( const int numOfTiles );

    public:
        void        Draw( const gs::Camera& worldCamera ) const;
        cck::Globe  GenerateTerrain() const;
        void        Update();

    public:
        Globe();
    };
}

#endif // GS_GLOBE_H
