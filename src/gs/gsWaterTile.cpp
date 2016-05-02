#include "gsWaterTile.h"

void gs::WaterTile::UpdateAllBuffers(const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo) //const
{
    UpdatePositionBuffer(positionVbo);
    UpdateColorBuffer(colorVbo);
    UpdateFogBuffer(fogVbo);
}

gs::WaterTile::WaterTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3f& centroid)
    :   gs::Tile(gs::Tile::Type::WATER, vertices, centroid, 0.0),
        ice(false)
{
    color = gs::Vec3f(0.09f, 0.13f, 0.33f);
}
