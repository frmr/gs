#include "gsBiomeTextureGenerator.h"
#include "gsBoundingBox.h"
#include "gsGlobe.h"
#include "gsLandTile.h"
#include "gsMath.h"
#include "gsRandomRange.h"
#include "gsTileGroupManager.h"

#include "../EasyBMP/EasyBMP.h"

#include <ctime>
#include <unordered_map>
#include <map>
#include <array>

gs::BiomeTextureGenerator gs::LandTile::biomeTextureGenerator;

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

bool gs::LandTile::CheckCoordIsNearCoast(const gs::Vec3d & coord) const
{
	constexpr double coastEdgeLimit = 0.004;
	constexpr double coastCornerLimit = 0.008;

	assert(coastCornerLimit > coastEdgeLimit);

	if (waterLinks.empty())
	{
		return false;
	}

	std::vector<double> innerDistances;

	for (const auto& link : waterLinks)
	{
		const double linkDistance = (gs::ClosestPointOnLine<gs::Vec3d>(link.edge->v0->position, link.edge->vec, coord, true) - coord).Length();
		if (linkDistance <= coastEdgeLimit)
		{
			return true;
		}
		else if (linkDistance <= coastCornerLimit)
		{
			innerDistances.push_back(coastCornerLimit - linkDistance);
		}
	}

	if (innerDistances.size() >= 2)
	{
		double sum = 0.0;
		for (const auto& dist : innerDistances)
		{
			sum += dist * dist;
		}

		if (std::sqrt(sum) > coastCornerLimit - coastEdgeLimit)
		{
			return true;
		}
	}

	 for (const auto& link : waterLinks)
	 {
		 if ((coord - link.edge->v0->position).Length() < coastEdgeLimit)
		 {
			 return true;
		 }

		 if ((coord - link.edge->v1->position).Length() < coastEdgeLimit)
		 {
			 return true;
		 }
	 }

	 return false;
}

void gs::LandTile::GenerateTexture()
{
    //TODO: Make this safer by checking for presence of first and second vertices

    //reference u-axis is from v0 to v1
    const gs::Vec3d refAxisU = (gs::Vec3d) (vertices[1]->position - vertices[0]->position).Unit();
    //reference v-axis is the cross-product of u-axis and the tile normal
    const gs::Vec3d refAxisV = gs::Cross<double>(refAxisU, normal).Unit(); //TODO: Second argument might actually be worldVertices[0]->position

    vector<gs::Vec2d> relativeCoords;
    relativeCoords.reserve(vertices.size());

    //use reference axes to compute relative coordinates of each world vertex
    for (const auto& vert : vertices)
    {
        relativeCoords.emplace_back(gs::Dot<double>(refAxisU, (gs::Vec3d) (vert->position - vertices.front()->position)),
                                     gs::Dot<double>(refAxisV, (gs::Vec3d) (vert->position - vertices.front()->position)));
    }

    gs::BoundingBox<gs::Vec2d> boundingBox(relativeCoords);

    //shift coordinates by bounding box minimum
    for (auto& coord : relativeCoords)
    {
        coord -= boundingBox.minCoord;
    }
    boundingBox.maxCoord -= boundingBox.minCoord;
	boundingBox.minCoord = gs::Vec2d();

    constexpr int pixelsPerUnit = 1000;
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
				texture->SetColor(x, y, pixelWorldCoord * 255.0);
				//texture->SetColor(x, y, gs::Color(255,0,0));
				continue;
			}


			//calculate distance to each notable edge
			vector<double> notableDistances;
			notableDistances.reserve(notableLinks.size());
			for (const auto& link : notableLinks)
			{
				notableDistances.push_back((gs::ClosestPointOnLine<gs::Vec3d>(link.edge->v0->position, link.edge->vec, pixelWorldCoord, true) - pixelWorldCoord).Length());
			}

            //TODO: Use iterators
			bool colorSet = false;
			for (int i = 0; i < notableLinks.size(); ++i)
			{
				if (notableLinks[i].edge->IsRiver())
				{
					if (notableDistances[i] < riverLimit)
					{
						texture->SetColor(x, y, gs::Color(0, 0, 255));
						colorSet = true;
						break;
					}
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
				const gs::Color sample = biomeTextureGenerator.Sample(pixelWorldCoord, element.first.first, element.first.second);
				tempColor += gs::Vec3d((double)sample.r * factor, (double)sample.g * factor, (double)sample.b * factor);
				factorSum += factor;
			}

			texture->SetColor(x, y, gs::Color(tempColor / factorSum));
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

void gs::LandTile::SetBiome(const gs::LandTile::Biome newBiome)
{
    biome = newBiome;
}

void gs::LandTile::SetCulture(const shared_ptr<Culture> newCulture)
{
    culture = newCulture;
}

bool gs::LandTile::SpawnRiver(const int newRiverId, gs::RandomRange<double>& rand)
{
    double probability = 0.0;
    if (terrain == gs::LandTile::Terrain::PLAINS)
    {
        probability = 0.05;
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

gs::LandTile::LandTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid, const double height, const int regionId)
    :   gs::Tile(gs::Tile::Type::LAND, vertices, centroid, height),
        regionId(regionId),
        terrain(DetermineTerrain()),
        forested(false),
        biome(gs::LandTile::Biome::UNASSIGNED)
{
    if (terrain == gs::LandTile::Terrain::PLAINS)
    {
        color = gs::Vec3d(0.0f, 0.8f, 0.0f);
    }
    else if (terrain == gs::LandTile::Terrain::HILLS)
    {
        color = gs::Vec3d(0.0f, 0.4f, 0.0f);
    }
    else if (terrain == gs::LandTile::Terrain::MOUNTAINS)
    {
        color = gs::Vec3d(0.4, 0.4, 0.4);
    }
}

gs::LandTile::~LandTile()
{
    DeleteLocalTextureData();
}
