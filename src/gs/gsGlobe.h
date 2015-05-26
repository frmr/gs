#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <vector>

#include <GL/gl3w.h>

#include "gsMatrixStack.h"
#include "gsProjectionMatrix.h"
#include "gsShader.h"

#include "../matrix/Matrices.h"

using std::vector;

namespace gs
{
    class Globe
    {
    private:
        GLuint              vao;
        GLuint              vbo;
        gs::Shader          shader;
        GLint               modelViewLocation;
        GLint               projectionLocation;
        //vector<gs::Vec3>    vertices;
        //vector<gs::Tile>    tiles;
        //vector<gs::Edge>    edges;

    public:
        void Draw( const gs::MatrixStack<Matrix4>& modelView, const gs::ProjectionMatrix& projection ) const;

    public:
        Globe();
    };
}

#endif // GS_GLOBE_H
