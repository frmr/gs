#ifndef GS_TILE_BUFFER_H
#define GS_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include "gsShader.h"
#include "gsVec3.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace gs
{
    class TileBuffer
    {
    protected:
        GLuint  vao;
        GLuint  positionVbo;
        GLuint  colorVbo;
        GLuint  fogVbo;
        GLuint  indexBuffer;

    private:
        void InitIndexBuffer(const vector<GLuint>& indexVector) const;

    protected:
        GLuint CreateVbo(const int elements, const int components, gs::Shader& shader, const string& name);

    public:
        void Bind() const;

        bool Update(const GLuint tileIndex, const gs::Vec3f& newColor, const bool newFog);

    public:
        TileBuffer(const int size, gs::Shader& shader, const vector<GLuint>& indexVector);
    };
}

#endif // GS_TILE_BUFFER_H
