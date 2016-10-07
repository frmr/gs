#pragma once

#include "gsLandTile.hpp"
#include "gsTexture.hpp"
#include "gsTileGroup.hpp"
#include "gsWaterTile.hpp"

#include <iostream>
#include <vector>

using std::cerr;
using std::endl;
using std::vector;

namespace gs
{
    class TileGroupManager
    {
    private:
        GLint                   textureDim;
        vector<gs::TileGroup>	landTileGroups;
        vector<gs::TileGroup>	waterTileGroups;

    public:
        void Add(const gs::LandTilePtr& landTile);
        void Add(const gs::WaterTilePtr& waterTile);
        void DrawAll() const;
		void DeleteLocalTextureData();
        void DrawLandTileGroups() const;
        void DrawWaterTileGroups() const;
        void PushLastLandTexture();
		void PushLastWaterTexture();
        void SetTextureSize(const GLint newTextureDim);
        void WriteTileGroupsToFile() const;

    public:
        explicit TileGroupManager(const GLint textureDim = 1024);
    };
}