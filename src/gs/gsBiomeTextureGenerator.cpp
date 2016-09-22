#include "gsBiomeTextureGenerator.h"

gs::DesertTextureGenerator		gs::BiomeTextureGenerator::desert;
gs::GrasslandTextureGenerator	gs::BiomeTextureGenerator::grassland;
gs::SemiAridTextureGenerator	gs::BiomeTextureGenerator::semiArid;
gs::TundraTextureGenerator		gs::BiomeTextureGenerator::tundra;

gs::Color& gs::BiomeTextureGenerator::Sample(const gs::Vec3d& coord, const gs::LandTile::Biome biome, const gs::LandTile::Terrain terrain) const
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

gs::BiomeTextureGenerator::BiomeTextureGenerator()
{
}
