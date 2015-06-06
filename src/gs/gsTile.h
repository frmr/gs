#ifndef GS_TILE_H
#define GS_TILE_H

#include <GL/gl3w.h>
#include <vector>

#include "../cck/cck.h"
#include "gsLink.h"
#include "gsVec3.h"
#include "gsVertex.h"

using std::vector;

namespace gs
{
    class Link;
    class Vertex;

    class Tile
    {
    public:

        bool visited; //delete
    private:
        const GLuint                            bufferOffset;
        const vector<shared_ptr<gs::Vertex>>    vertices;
        gs::Vec3f                               color;
        bool                                    fog;

        vector<gs::Link>                        links;

        //owner
        //controller
    public:
        //void Update( )
        bool AddLink( const gs::Link& link );
        void InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector );// const;

        //void MapToTexture( vector<gs::Texture>& textures );


        void SetColor(); //delete

    public:
        Tile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const cck::Globe& terrain );
    };
}

#endif // GS_TILE_H
