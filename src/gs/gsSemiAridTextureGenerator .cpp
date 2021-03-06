#include "gsSemiAridTextureGenerator.hpp"

#include <algorithm>

const std::vector<gs::Vec3d> gs::SemiAridTextureGenerator::colors = { gs::Vec3d(132.0f, 122.0f, 102.0f),
																	gs::Vec3d(84.0f, 86.0f, 71.0f),
																	gs::Vec3d(51.0f, 64.0f, 78.0f),
																	gs::Vec3d(210.0f, 210.0f, 210.0f),
																	gs::Vec3d(250.0f, 250.0f, 250.0f) };

const std::vector<float> gs::SemiAridTextureGenerator::limits = { 0.3f, 0.5f, 0.7f, 0.8f };

gs::Color gs::SemiAridTextureGenerator::Sample(const gs::Vec3d& coord, const gs::LandTile::Terrain terrain)
{
	float minHeight = 0.0f;
	float maxHeight = 0.0f;

	if (terrain == gs::LandTile::Terrain::PLAINS)
	{
		minHeight = 0.0f;
		maxHeight = 0.2f;
	}
	else if (terrain == gs::LandTile::Terrain::HILLS)
	{
		minHeight = 0.3f;
		maxHeight = 0.5f;
	}
	else
	{
		minHeight = 0.3f;
		maxHeight = 1.0f;
	}

	const float noiseValA = minHeight + noiseA.GetGradientFractal(float(coord.x) * 2560.0f, float(coord.y) * 2560.0f, float(coord.z) * 2560.0f) * (maxHeight - minHeight);
	const float noiseValB = minHeight + noiseB.GetGradientFractal(float(coord.x) * 2560.0f, float(coord.y) * 2560.0f, float(coord.z) * 2560.0f) * (noiseValA - minHeight);

	//const double funcVal = 1.0 - (noiseValA - noiseValB * 0.5);
	//const double funcVal = minHeight + (1.0 - (noiseValA - noiseValB * 0.5)) * (maxHeight - minHeight);
	const float funcVal = noiseValA - noiseValB * 0.7f;

	//heightBuffer.At(x, y) = std::min(std::max(funcVal * scale, 0.0), 255.0);

	if (funcVal < limits[0])
	{
		const float interp = funcVal / limits[0];
		return colors[0] + (colors[1] - colors[0]) * interp;
	}
	else if (funcVal < limits[1])
	{
		const float interp = (funcVal - limits[0]) / (limits[1] - limits[0]);
		return colors[1] + (colors[2] - colors[1]) * interp;
	}
	else if (funcVal < limits[2])
	{
		const float interp = (funcVal - limits[1]) / (limits[2] - limits[1]);
		return colors[2] + (colors[3] - colors[2]) * interp;
	}
	else
	{
		const float interp = (funcVal - limits[2]) / (limits[3] - limits[2]);
		return colors[3] + (colors[4] - colors[3]) * interp;
	}
}

gs::SemiAridTextureGenerator::SemiAridTextureGenerator()
	: noiseA(0), noiseB(0)
{
	noiseA.SetNoiseType(FastNoise::GradientFractal);
	noiseA.SetFractalType(FastNoise::FractalType::RigidMulti);
	noiseA.SetFrequency(0.025f);
	noiseA.SetFractalOctaves(1);

	noiseB.SetNoiseType(FastNoise::GradientFractal);
	noiseB.SetFractalType(FastNoise::FractalType::RigidMulti);
	noiseB.SetFrequency(0.08f);
	noiseB.SetFractalOctaves(2);
	noiseB.SetFractalLacunarity(3.0f);
	noiseB.SetFractalGain(0.6f);
}
