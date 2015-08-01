#ifndef GS_TILE_H
#define GS_TILE_H

#include "../gl3w/gl3w.h"
#include <vector>

#include "gsLink.h"
#include "gsVec3.h"
#include "gsVertex.h"

using std::vector;

namespace gs
{
    class LandTile;
    class Vertex;
    class WaterTile;

    class Tile
    {
    public:
        enum class Type
        {
            UNASSIGNED,
            LAND,
            WATER
        };

    public:
        const int                               id;

    protected:
        static int                              idCounter;
        Type                                    surface;
        //const gs::Vec3f                         centroid;
        const gs::Vec3f                         normal;
        const double                            height;

        const vector<shared_ptr<gs::Vertex>>    vertices;
        GLuint                                  bufferOffset;
        GLuint                                  indexBufferOffset;
        gs::Vec3f                               color;
        bool                                    fog;

        vector<gs::Link<gs::Tile>>              allLinks;
        vector<gs::Link<gs::LandTile>>          landLinks;
        vector<gs::Link<gs::WaterTile>>         waterLinks;

        //owner
        //controller
    protected:
        void    InitColorBuffer( const GLuint colorVbo );
        void    InitFogBuffer( const GLuint fogVbo );
        void    InitPositionBuffer( const GLuint positionVbo );

    public:
        void    AddVerticesToIndexVector( vector<GLuint>& indexVector );

        bool    AddLink( const gs::Link<gs::Tile>& link );
        bool    AddLink( const gs::Link<gs::LandTile>& link );
        bool    AddLink( const gs::Link<gs::WaterTile>& link );

        GLuint  GetBufferEnd() const;
        GLuint  GetBufferOffset() const;
        double  GetHeight() const;
        GLuint  GetIndexBufferEnd() const;
        GLuint  GetIndexBufferOffset() const;
        size_t  GetNumOfVertices() const;
        Type    GetSurface() const;

        GLuint  SetBufferOffset( const GLuint offset );

    protected:
        Tile( const Type surface, const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height );

    public:
        //Tile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices );
        virtual ~Tile();
    };

    typedef shared_ptr<gs::Tile> TilePtr;
}

#endif // GS_TILE_H
