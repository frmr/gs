#ifndef GS_TILE_H
#define GS_TILE_H

#include <GL/gl3w.h>
#include <vector>

#include "../cck/cck.h"
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

        //owner
        //controller
    public:
        //void Update( )
        void InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector ) const;

    public:
        Tile( const int bufferOffset, const vector<gs::Vec3f>& vertices, const cck::Globe& terrain, gs::RandomRange& randColor );
    };
}

#endif // GS_TILE_H
