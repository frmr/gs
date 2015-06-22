#ifndef GS_TILE_TEXTURE_H
#define GS_TILE_TEXTURE_H

#include <limits>

#include "gsMath.h"
#include "gsVec2.h"

#include "../EasyBMP/EasyBMP.h"

namespace gs
{
    class TileTexture
    {
    private:
        int width;
        int height;
        gs:Vec2f texCoords;

        BMP file;
    private:
        static gs::Vec2f ComputeRelativePoint( const gs::Vec3f& uAxis, const gs::Vec3f& vAxis, const gs::Vec3f& p )
        {
            gs::Vec3f pVec = point - vert0;

            float dotUU = VectorDot( uAxis, uAxis );
            float dotUV = VectorDot( uAxis, vAxis );
            float dotUP = VectorDot( uAxis, p );
            float dot22 = VectorDot( vec02, vec02 );
            float dot2p = VectorDot( vec02, pVec );

            float invDenom = 1.0f / ( dot11 * dot22 - dot12 * dot12 );
            float u = ( dot22 * dot1p - dot12 * dot2p ) * invDenom;
            float v = ( dot11 * dot2p - dot12 * dot1p ) * invDenom;
        }

    public:
        TileTexture( const vector<gs::VertexPtr>& worldVertices, const vector<gs::Link<gs::LandTile>>& links )
        {
            //reference u-axis is from v0 to v1
            gs::Vec3f refAxisU = ( worldVertices[1]->position - worldVertices[0]->position ).Unit();
            gs::Vec3f normal = worldVertices[0]->position;
            normal.Unit();
            //reference v-axis is the cross-product of u-axis and the tile normal
            gs::Vec3f refAxisV = gs::Cross( refAxisU, normal ).Unit();

            vector<gs::Vec2f> relativeCoords;
            relativeCoords.reserve( worldVertices.size() );

            //use reference axes to compute relative coordinates of each world vertex
            for ( const auto& vert : worldVertices )
            {
                relativeCoords.push_back( ( vert->position - worldVertices[0]->position ).ToVec2() );
            }

            //compute bounding box
            gs::Vec2f minCoord = gs::Vec2f( std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
            gs::Vec2f maxCoord = gs::Vec2f( std::numeric_limits<float>::min(), std::numeric_limits<float>::min() );
            for ( const auto& coord : relativeCoords )
            {
                if ( coord.x < minCoord.x ) { minCoord.x = coord.x; }
                if ( coord.y < minCoord.y ) { minCoord.y = coord.y; }
                if ( coord.x > maxCoord.x ) { maxCoord.x = coord.x; }
                if ( coord.y > maxCoord.y ) { maxCoord.y = coord.y; }
            }

            //shift coordinates by bounding box minimum
            for ( auto& coord : relativeCoord )
            {
                coord += minCoord;
            }
            minCoord += minCoord; //should be (0,0)

        }
    };
}
#endif // GS_TILE_TEXTURE_H
