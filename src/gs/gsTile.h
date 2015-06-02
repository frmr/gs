#ifndef GS_TILE_H
#define GS_TILE_H

#include <GL/gl3w.h>
#include <vector>

#include "gsRandomRange.h"
#include "gsVec3.h"

using std::vector;

namespace gs
{
    class Tile
    {
    private:
        const GLuint            bufferOffset;
        const vector<gs::Vec3f> vertices;
        gs::Vec3f               color;
        bool                    fog;

        //create new tiles one by one, keeping track of number of vertices
        //save offset when it first comes through after visiting other Tiles

        //have Globe allocate memory for all the buffers once its counted the required number
        //go through all the tiles a second time, filling the buffers (InitBuffers())
        //send buffers to vram

        //owner
        //controller
    public:
        //void Update( )
        void InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector ) const;

    public:
        Tile( const int bufferOffset, const vector<gs::Vec3f>& vertices, gs::RandomRange& randColor );
    };
}

#endif // GS_TILE_H
