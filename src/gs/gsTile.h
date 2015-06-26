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

        bool visited; //delete

    public:
        const int                               id;

    protected:
        static int                              idCounter;
        Type                                    surface;
        const gs::Vec3f                         centroid;
        const double                            height;

        const GLuint                            bufferOffset;
        const vector<shared_ptr<gs::Vertex>>    vertices;
        gs::Vec3f                               color;
        bool                                    fog;

        vector<gs::Link<gs::Tile>>              allLinks;
        vector<gs::Link<gs::LandTile>>          landLinks;
        vector<gs::Link<gs::WaterTile>>         waterLinks;

        //owner
        //controller
    public:
        //void Update( )
        bool    AddLink( const gs::Link<gs::Tile>& link );
        bool    AddLink( const gs::Link<gs::LandTile>& link );
        bool    AddLink( const gs::Link<gs::WaterTile>& link );

        double  GetHeight() const;
        void    InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint texCoordVbo, const GLuint fogVbo, vector<GLuint>& indexVector );// const;

        //void  MapToTexture( vector<gs::Texture>& textures );

        Type    GetSurface() const;

    protected:
        Tile( const Type surface, const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid, const double height );

    public:
        //Tile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices );
        virtual ~Tile();
    };

    typedef shared_ptr<gs::Tile> TilePtr;
}

#endif // GS_TILE_H
