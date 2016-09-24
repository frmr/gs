#include "gsTileGroupManager.h"
#include "gsLandTile.h"

void gs::TileGroupManager::Add(const LandTilePtr& landTile)
{
    if (landTileGroups.empty())
    {
        landTileGroups.emplace_back(0, textureDim);
    }

    if (!landTileGroups.back().Add(landTile))
    {
		landTileGroups.back().PushTexture();
		landTileGroups.back().DeleteLocalTextureData();
        landTileGroups.emplace_back(landTileGroups.back().GetBufferEnd() + 1, textureDim);
        if (!landTileGroups.back().Add(landTile))
        {
            cerr << "gs::TileGroupManager::Add() in src/gs/gsTileGroupManager.h: Could not add tile texture to empty tile group. Tile texture is too large." << endl;
        }
    }
}

void gs::TileGroupManager::Add(const WaterTilePtr& waterTile)
{
	if (waterTileGroups.empty())
	{
		waterTileGroups.emplace_back(0, textureDim);
	}

	if (!waterTileGroups.back().Add(waterTile))
	{
		waterTileGroups.back().PushTexture();
		waterTileGroups.back().DeleteLocalTextureData();
		waterTileGroups.emplace_back(waterTileGroups.back().GetBufferEnd() + 1, textureDim);
		if (!waterTileGroups.back().Add(waterTile))
		{
			cerr << "gs::TileGroupManager::Add() in src/gs/gsTileGroupManager.h: Could not add tile texture to empty tile group. Tile texture is too large." << endl;
		}
	}
}

void gs::TileGroupManager::DeleteLocalTextureData()
{
	for (auto& group : landTileGroups)
	{
		group.DeleteLocalTextureData();
	}

	for (auto& group : waterTileGroups)
	{
		group.DeleteLocalTextureData();
	}
}

void gs::TileGroupManager::DrawAll() const
{
	DrawLandTileGroups();
	DrawWaterTileGroups();
}

void gs::TileGroupManager::DrawLandTileGroups() const
{
    for (const auto& group : landTileGroups)
    {
        group.Draw();
    }
}

void gs::TileGroupManager::DrawWaterTileGroups() const
{
	for (const auto& group : waterTileGroups)
	{
		group.Draw();
	}
}

void gs::TileGroupManager::PushLastLandTexture()
{
	landTileGroups.back().PushTexture();
	landTileGroups.back().DeleteLocalTextureData();
}

void gs::TileGroupManager::PushLastWaterTexture()
{
	waterTileGroups.back().PushTexture();
	waterTileGroups.back().DeleteLocalTextureData();
}

void gs::TileGroupManager::SetTextureSize(const GLint newTextureDim)
{
    textureDim = newTextureDim;
}

void gs::TileGroupManager::WriteTileGroupsToFile() const
{
    for (const auto& group : landTileGroups)
    {
        group.WriteToFile();
    }

	for (const auto& group : waterTileGroups)
	{
		group.WriteToFile();
	}
}

gs::TileGroupManager::TileGroupManager(const GLint textureDim)
    :   textureDim(textureDim)
{
}
