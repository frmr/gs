#ifndef GS_TILE_H
#define GS_TILE_H

#include <GL/gl3w.h>
#include <vector>

#include "../cck/cck.h"
#include "gsLink.h"
#include "gsVec3.h"

using std::vector;

namespace gs
{
    class Link;

    class Tile
    {
    public:
        enum class Climate
        {

        };
        bool visited;
    private:
        const GLuint            bufferOffset;
        const vector<gs::Vec3f> vertices;
        gs::Vec3f               color;
        bool                    fog;

        vector<gs::Link>        links;

        //owner
        //controller
    public:
        //void Update( )
        bool AddLink( const gs::Link& link );
        void InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector );// const;

        //void MapToTexture( vector<gs::Texture>& textures );


        void SetColor(); //delete

    public:
        Tile( const int bufferOffset, const vector<gs::Vec3f>& vertices, const cck::Globe& terrain );
    };
}

#endif // GS_TILE_H
