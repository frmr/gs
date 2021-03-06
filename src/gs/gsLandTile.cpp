#include "gsLandTextureGenerator.hpp"
#include "gsBoundingBox.hpp"
#include "gsGlobe.hpp"
#include "gsLandTile.hpp"
#include "gsMath.hpp"
#include "gsRandomRange.hpp"
#include "gsTileGroupManager.hpp"

#include "../EasyBMP/EasyBMP.h"

#include <ctime>
#include <unordered_map>
#include <map>
#include <array>

gs::LandTextureGenerator gs::LandTile::landTextureGenerator;

gs::LandTile::Terrain gs::LandTile::DetermineTerrain() const
{
    if (height < 0.25)
    {
        return gs::LandTile::Terrain::PLAINS;
    }
    else if (height < 0.7)
    {
        return gs::LandTile::Terrain::HILLS;
    }
    else
    {
        return gs::LandTile::Terrain::MOUNTAINS;
    }
}

gs::UnitF gs::LandTile::GetBiomeEnvironmentRating() const
{
	switch (biome)
	{
	case Biome::UNASSIGNED:	return gs::UnitF(0.0f, false);
	case Biome::DESERT:		return gs::UnitF(0.4f, false);
	case Biome::ICE:		return gs::UnitF(0.4f, false);
	case Biome::TUNDRA:		return gs::UnitF(0.6f, false);
	case Biome::SEMI_ARID:	return gs::UnitF(0.8f, false);
	case Biome::GRASSLAND:	return gs::UnitF(1.0f, false);
	};

	assert(false);

	return UnitF(0.0, false);
}

gs::UnitF gs::LandTile::GetTerrainEnvironmentRating() const
{
	switch (terrain)
	{
	case Terrain::LAKE:			return gs::UnitF(0.0f, false);
	case Terrain::MOUNTAINS:	return gs::UnitF(0.5f, false);
	case Terrain::HILLS:		return gs::UnitF(0.8f, false);
	case Terrain::PLAINS:		return gs::UnitF(1.0f, false);
	}

	assert(false);

	return gs::UnitF(0.0, false);
}

gs::UnitF gs::LandTile::GetLatitudeEnvironmentRating() const
{
	const float latitude = float(std::abs(cck::Vec3(center.x, center.z, center.y).ToGeographic().latRadians));
	
	if (latitude < cck::quarterPi)
	{
		return gs::UnitF(0.75f + 0.25f * latitude / float(cck::quarterPi), false);
	}
	else
	{
		return gs::UnitF(1.0f - std::min(1.0f, (latitude - float(cck::quarterPi)) / ((0.9f * float(cck::halfPi) - float(cck::quarterPi)))), false);
	}
}

bool gs::LandTile::CheckCoordIsNearCoast(const gs::Vec3d& coord) const
{
	constexpr double coastEdgeLimit = 0.004;
	constexpr double coastCornerLimit = 0.008;

	assert(coastCornerLimit > coastEdgeLimit);

	if (waterLinks.empty())
	{
		return false;
	}

	std::vector<double> distances;
	distances.reserve(waterLinks.size());
	
	std::vector<double> reverseDistances;
	reverseDistances.reserve(waterLinks.size());

	for (const auto& link : waterLinks)
	{
		const gs::Vec3d closestPoint = gs::ClosestPointOnLine<gs::Vec3d>(link.edge->v0->position, link.edge->vec, coord, true);
		const double distance = (closestPoint - coord).Length();

		if (distance <= coastCornerLimit)
		{
			distances.push_back(distance);
			reverseDistances.push_back(coastCornerLimit - distance);
		}
	}

	if (distances.empty())
	{
		return false;
	}
	else if (distances.size() == 1)
	{
		return (distances.front() < coastEdgeLimit);
	}
	else
	{
		double sum = 0.0;
	
		for (const auto& dist : reverseDistances)
		{
			sum += dist * dist;
		}
	
		return (std::sqrt(sum) > coastCornerLimit - coastEdgeLimit);
	}
}

void gs::LandTile::CalculateEnvironmentRating()
{
	const UnitF biomeValue = GetBiomeEnvironmentRating();
	const UnitF terrainValue = GetTerrainEnvironmentRating();
	const UnitF latitudeValue = GetLatitudeEnvironmentRating();
	const UnitF coastValue(waterLinks.empty() ? 0.8f : 1.0f, false);

	UnitF riverValue(0.8f, false);

	for (const auto& link : landLinks)
	{
		if (link.edge->IsRiver())
		{
			riverValue = 1.0f;
			break;
		}
	}

	environmentRating = biomeValue * terrainValue * latitudeValue * coastValue * riverValue;
	color = gs::Vec3d(environmentRating, 0, 0);
}

void gs::LandTile::CalculateMovementRating()
{
	switch (terrain)
	{
	case Terrain::LAKE:			movementRating = 1.0f;	break;
	case Terrain::PLAINS:		movementRating = 1.0f;	break;
	case Terrain::HILLS:		movementRating = 0.8f;	break;
	case Terrain::MOUNTAINS:	movementRating = 0.2f;	break;
	}

	UnitF neighborsMovementRating(1.0f, false);

	for (const auto& link : landLinks)
	{
		if (link.target->terrain == Terrain::HILLS)
		{
			neighborsMovementRating -= 0.025f;
		}
		else if (link.target->terrain == Terrain::MOUNTAINS)
		{
			neighborsMovementRating -= 0.1f;
		}

		neighborsMovementRating = std::max(0.2f, float(neighborsMovementRating));
	}

	movementRating *= neighborsMovementRating;

	//color = gs::Vec3d(movementRating, 0, 0);
}

void gs::LandTile::GenerateTexture()
{
    //TODO: Make this safer by checking for presence of first and second vertices

    //reference u-axis is from v0 to v1
    const gs::Vec3d refAxisU = (vertices[1]->position - vertices[0]->position).Unit();
    //reference v-axis is the cross-product of u-axis and the tile normal
    const gs::Vec3d refAxisV = gs::Cross<double>(refAxisU, normal).Unit(); //TODO: Second argument might actually be worldVertices[0]->position

    vector<gs::Vec2d> relativeCoords;
    relativeCoords.reserve(vertices.size());

    //use reference axes to compute relative coordinates of each world vertex
    for (const auto& vert : vertices)
    {
        relativeCoords.emplace_back(gs::Dot<double>(refAxisU, vert->position - vertices.front()->position),
                                     gs::Dot<double>(refAxisV, (vert->position - vertices.front()->position)));
    }

    gs::BoundingBox<gs::Vec2d> boundingBox(relativeCoords);

    //shift coordinates by bounding box minimum
    for (auto& coord : relativeCoords)
    {
        coord -= boundingBox.minCoord;
    }

    boundingBox.maxCoord -= boundingBox.minCoord;
	boundingBox.minCoord = gs::Vec2d();

    constexpr int pixelsPerUnit = 1024;
	constexpr uint8_t edgeCushion = 32;

    const int width = std::max((int) (boundingBox.maxCoord.x * pixelsPerUnit), 1) + edgeCushion * 2;
    const int height = std::max((int) (boundingBox.maxCoord.y * pixelsPerUnit), 1) + edgeCushion * 2;

    texture = std::make_shared<gs::Texture>(width, height);

    texCoords.reserve(vertices.size());
    for (const auto& coord : relativeCoords)
    {
        texCoords.emplace_back(coord.x * pixelsPerUnit + edgeCushion, coord.y * pixelsPerUnit + edgeCushion);
    }

	constexpr double riverLimit = 0.002;
	constexpr double blendLimit = 0.008;

	const gs::Vec3d xJump = refAxisU / (double) pixelsPerUnit;
	const gs::Vec3d yJump = refAxisV / (double) pixelsPerUnit;

	const gs::Vec3d pixelOriginWorldCoord = vertices[0]->position - (xJump * (texCoords.front().x - edgeCushion)) - (yJump * (texCoords.front().y - edgeCushion)); //world coordinate of pixel (0,0)

	const gs::Color noLandColor(255, 0, 255);

	//create sublist of links to tiles that are rivers or need blending
	vector<gs::Link<gs::LandTile>> notableLinks;
	for (const auto& link : landLinks)
	{
		//if (link.target->biome != this->biome || link.target->terrain != terrain || link.edge->IsRiver())
		if (link.target->biome != this->biome || link.target->terrain != this->terrain || link.edge->IsRiver())
		{
			notableLinks.push_back(link);
		}
	}

    //sample texture generator at each pixel
    for (int x = 0; x < width; ++x)
    {
        for (int  y = 0; y < height; ++y)
        {
			const gs::Vec3d pixelWorldCoord = (pixelOriginWorldCoord + xJump * (x - edgeCushion) + yJump * (y - edgeCushion)).Unit(); //TODO: Speed this up by using xJump and yJump to increment for each pixel

			if (CheckCoordIsNearCoast(pixelWorldCoord))
			{
				texture->SetColor(x, y, noLandColor);
				continue;
			}

			//calculate distance to each notable edge
			vector<double> notableDistances;
			notableDistances.reserve(notableLinks.size());
			for (const auto& link : notableLinks)
			{
				notableDistances.push_back((gs::ClosestPointOnLine<gs::Vec3d>(link.edge->v0->position, link.edge->vec, pixelWorldCoord, true) - pixelWorldCoord).Length());
			}

			//check for rivers
            //TODO: Use iterators
			bool colorSet = false;
			for (int i = 0; i < notableLinks.size(); ++i)
			{
				if (notableLinks[i].edge->IsRiver() && notableDistances[i] < riverLimit)
				{
					texture->SetColor(x, y, noLandColor);
					colorSet = true;
					break;
				}
			}

			if (colorSet)
			{
				continue;
			}

			for (const auto& vertex : vertices)
			{
				if (vertex->IsRiver() && (vertex->position - pixelWorldCoord).Length() < riverLimit)
				{
					texture->SetColor(x, y, noLandColor);
					colorSet = true;
					break;
				}
			}

			if (colorSet)
			{
				continue;
			}

			std::map<std::pair<gs::LandTile::Biome, gs::LandTile::Terrain>, double> distanceMap;

			distanceMap.emplace(std::make_pair(this->biome, this->terrain), 0.0);

			for (int i = 0; i < notableLinks.size(); ++i)
			{
				if (notableDistances[i] >= blendLimit)
				{
					continue;
				}

				const gs::LandTile::Biome targetBiome = notableLinks[i].target->biome;
				const gs::LandTile::Terrain targetTerrain = notableLinks[i].target->terrain;

				const std::pair<gs::LandTile::Biome, gs::LandTile::Terrain> key(notableLinks[i].target->biome, notableLinks[i].target->terrain);

				if (distanceMap.count(key))
				{
					if (notableDistances[i] < distanceMap.at(key))
					{
						distanceMap.at(key) = notableDistances[i];
					}
				}
				else
				{
					distanceMap.emplace(key, notableDistances[i]);
				}
			}

			gs::Vec3d tempColor;
			double factorSum = 0.0;

			for (auto& element : distanceMap)
			{
				const double factor = (blendLimit - element.second) / blendLimit;
				const gs::Color sample = landTextureGenerator.Sample(pixelWorldCoord, element.first.first, element.first.second);
				tempColor += gs::Vec3d((double)sample.r * factor, (double)sample.g * factor, (double)sample.b * factor);
				factorSum += factor;
			}

			texture->SetColor(x, y, gs::Color(tempColor / factorSum));
        }
    }

	constexpr int aaRadius = 2;

	//blend coast
	for (int x = aaRadius; x < width - aaRadius; ++x)
	{
		for (int y = aaRadius; y < height - aaRadius; ++y)
		{
			const gs::Color oldColor = texture->GetColor(x, y);

			if (oldColor != noLandColor)
			{
				constexpr double factorIncrement = 0.125;
				double waterFactor = 0.0;

				for (int sampleX = x - aaRadius; sampleX <= x + aaRadius; ++sampleX)
				{
					for (int sampleY = y - aaRadius; sampleY <= y + aaRadius; ++sampleY)
					{
						if (!(x == sampleX && y == sampleY) && texture->GetColor(sampleX, sampleY) == noLandColor)
						{
							waterFactor += factorIncrement;
						}
					}
				}

				waterFactor = std::min(waterFactor, 1.0);

				const gs::Color waterColor = waterTextureGenerator.Sample();

				const gs::Vec3d tempColor(
					(double)waterColor.x * waterFactor + (double)oldColor.x * (1.0 - waterFactor),
					(double)waterColor.y * waterFactor + (double)oldColor.y * (1.0 - waterFactor),
					(double)waterColor.z * waterFactor + (double)oldColor.z * (1.0 - waterFactor));

				texture->SetColor(x, y, tempColor);
			}
		}
	}

	//color water
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const gs::Color oldColor = texture->GetColor(x, y);
			const gs::Color waterColor = waterTextureGenerator.Sample();

			if (texture->GetColor(x, y) == noLandColor)
			{
				texture->SetColor(x, y, waterTextureGenerator.Sample());
			}
		}
	}
}

vector<shared_ptr<gs::LandTile>> gs::LandTile::GetUnassignedBiomeNeighbors() const
{
    vector<shared_ptr<gs::LandTile>> unassignedNeighbors;
    for (const auto& link : landLinks)
    {
        if (link.target->GetBiome() == gs::LandTile::Biome::UNASSIGNED)
        {
            unassignedNeighbors.push_back(link.target);
        }
    }
    return unassignedNeighbors;
}

shared_ptr<gs::Culture> gs::LandTile::GetCulture() const
{
	return culture;
}

float gs::LandTile::GetEnvironmentRating() const
{
	return environmentRating;
}

float gs::LandTile::GetMovementRating() const
{
	return movementRating;
}

gs::LandTile::Biome gs::LandTile::GetBiome() const
{
    return biome;
}

bool gs::LandTile::HasUnassignedBiomeNeighbors() const
{
    for (const auto& link : landLinks)
    {
        if (link.target->GetBiome() == gs::LandTile::Biome::UNASSIGNED)
        {
            return true;
        }
    }
    return false;
}

bool gs::LandTile::HasUnassignedCultureNeighbors() const
{
	for (const auto& link : landLinks)
	{
		if (link.target->GetCulture() == nullptr)
		{
			return true;
		}
	}
	return false;
}

bool gs::LandTile::HasDifferentCultureNeighbors() const
{
	for (const auto& link : landLinks)
	{
		if (link.target->culture != this->culture)
		{
			return true;
		}
	}

	return false;
}

void gs::LandTile::SetBiome(const gs::LandTile::Biome newBiome)
{
    biome = newBiome;
}

void gs::LandTile::SetCulture(const shared_ptr<Culture> newCulture)
{
    culture = newCulture;
	color = gs::Vec3d(double(culture->color.r) / 255.0, double(culture->color.g) / 255.0, double(culture->color.b) / 255.0);
}

bool gs::LandTile::SpawnRiver(const int newRiverId, gs::RandomRange<double>& rand)
{
    double probability = 0.0;
    if (terrain == gs::LandTile::Terrain::PLAINS)
    {
        probability = 0.01;
    }
    else if (terrain == gs::LandTile::Terrain::HILLS)
    {
        probability = 0.5;
    }
    else if (terrain == gs::LandTile::Terrain::MOUNTAINS)
    {
        probability = 1.0;
    }

    if (rand.Sample() < probability)
    {
        return vertices[(int) (rand.Sample() * (double) vertices.size())]->SetRiver(newRiverId);
    }
    else
    {
        return false;
    }
}

gs::LandTile::LandTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid, const double height, const int regionId) :
		gs::Tile(gs::Tile::Type::LAND, vertices, centroid, height),
        regionId(regionId),
        terrain(DetermineTerrain()),
        forested(false),
        biome(gs::LandTile::Biome::UNASSIGNED),
		environmentRating(0.0, false),
		movementRating(0.0, false),
		culture(nullptr)
{
}

gs::LandTile::~LandTile()
{
    DeleteLocalTextureData();
}
