#include "gsWaterTile.h"

gs::WaterTile::WaterTile( const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices )
    :   gs::Tile( gs::Tile::Type::WATER, bufferOffset, vertices, 0.0 ),
        ice( false )
{
    color = gs::Vec3f( 0.0f, 0.0f, 1.0f );
}