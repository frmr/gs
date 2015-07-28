#include "gsWaterTile.h"

void gs::WaterTile::InitBuffers( const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo ) //const
{
    InitPositionBuffer( positionVbo );
    InitColorBuffer( colorVbo );
    InitFogBuffer( fogVbo );
}

gs::WaterTile::WaterTile( const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid )
    :   gs::Tile( gs::Tile::Type::WATER, vertices, centroid, 0.0 ),
        ice( false )
{
    color = gs::Vec3f( 0.0f, 0.0f, 1.0f );
}
