#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <string>
#include <vector>

#include <GL/gl3w.h>
#include "../matrix/Matrices.h"

#include "gsCamera.h"
#include "gsMatrixStack.h"
#include "gsShader.h"
#include "gsTile.h"

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

        //vector<gs::Vec3>    vertices;
        vector<gs::Tile>    tiles;
        //vector<gs::Edge>    edges;

    private:
        vector<gs::Tile>    ConstructTiles( const int quantity ) const;
        GLuint              CreateVbo( const void* data, const int size, const int components, const string& name );
        GLuint              CreateVbo( const int size, const int components, const string& name );

    public:
        void Draw( const gs::Camera& worldCamera ) const;
        void Update();

    public:
        Globe();
    };
}

#endif // GS_GLOBE_H
