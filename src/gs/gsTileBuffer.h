#ifndef GS_TILE_BUFFER_H
#define GS_TILE_BUFFER_H

#include "../gl3w/gl3w.h"

#include <string>

using std::string;

namespace gs
{
    class TileBuffer
    {
    protected:
        GLuint  vao;
        GLuint  positionVbo;
        GLuint  colorVbo;
        GLuint  fogVbo;

    protected:
        GLuint CreateVbo( const int elements, const int components, gs::Shader& shader, const string& name );

    public:
        void Bind();

        bool Update( const GLuint tileIndex, const gs::Vec3f& newColor, const bool newFog );

    public:
        TileBuffer( const int size, gs::Shader& shader );
    };
}

#endif // GS_TILE_BUFFER_H
