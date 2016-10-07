#pragma once

#include "gsTile.hpp"
#include "gsVertex.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace gs
{
	class LandTextureGenerator;
    class Tile;

    class WaterTile : public gs::Tile
    {
    private:
        bool ice;

    public:
        WaterTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid);
		void GenerateTexture();
    };

    typedef shared_ptr<gs::WaterTile> WaterTilePtr;
}