#include "gsLandTextureGenerator.h"

gs::DesertTextureGenerator		gs::LandTextureGenerator::desert;
gs::GrasslandTextureGenerator	gs::LandTextureGenerator::grassland;
gs::SemiAridTextureGenerator	gs::LandTextureGenerator::semiArid;
gs::TundraTextureGenerator		gs::LandTextureGenerator::tundra;

gs::Color gs::LandTextureGenerator::Sample(const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const
{
	if (biome == gs::LandTile::Biome::DESERT)
	{
		return desert.Sample(coord, terrain);
	}
	else if (biome == gs::LandTile::Biome::GRASSLAND)
	{
		return grassland.Sample(coord, terrain);
	}
	else if (biome == gs::LandTile::Biome::SEMI_ARID)
	{
		return semiArid.Sample(coord, terrain);
	}
	else if (biome == gs::LandTile::Biome::TUNDRA)
	{
		return tundra.Sample(coord, terrain);
	}
	else
	{
		return gs::Color(255, 0, 255);
	}
}

gs::LandTextureGenerator::LandTextureGenerator()
{
}
