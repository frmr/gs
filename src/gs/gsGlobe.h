#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <vector>

#include <GL/gl3w.h>
#include "../matrix/Matrices.h"

#include "gsCamera.h"
#include "gsMatrixStack.h"
#include "gsShader.h"



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
        void Draw( const gs::Camera& worldCamera ) const;
        void Update();

    public:
        Globe();
    };
}

#endif // GS_GLOBE_H
