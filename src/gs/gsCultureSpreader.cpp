#include "gsCultureSpreader.hpp"

#include <ctime>
#include <iostream>
#include <unordered_map>

using std::cerr;
using std::endl;

void gs::CultureSpreader::CleanFrontier()
{
    for (vector<gs::LandTilePtr>::iterator it = frontier.begin(); it != frontier.end();)
    {
        if ((*it)->HasUnassignedCultureNeighbors())
        {
			++it;
        }
        else
        {
			it = frontier.erase(it);
        }
    }
}

gs::LandTilePtr gs::CultureSpreader::SelectRandomTileFromVector(const vector<gs::LandTilePtr>& tiles, gs::RandomRange<double>& randomIndex) const
{
    return (tiles.empty()) ? nullptr : tiles[(int)(randomIndex.Sample() * (double)tiles.size())];
}

bool gs::CultureSpreader::Spread()
{
	bool spread = false;

	for (unsigned int i = 0; i < speed; ++i)
	{
		if (frontier.empty())
		{
			break;
		}

		std::unordered_map<int, gs::LandTilePtr> neighbors;

		for (const auto& frontierTile : frontier)
		{
			const std::vector<gs::Link<gs::LandTile>> landLinks = frontierTile->GetLandLinks();

			for (const auto& link : landLinks)
			{
				if (link.target->GetCulture() == nullptr && neighbors.count(link.target->id) == 0)// || neighbors.count(link.target->GetCulture()->id) == 0)
				{
					neighbors.emplace(link.target->id, link.target);
				}
			}
		}

		if (neighbors.empty())
		{
			break;
		}

		gs::LandTilePtr bestNeighbor = nullptr;
		float bestNeighborValue = std::numeric_limits<float>::lowest();

		for (const auto& neighborPair : neighbors)
		{
			const gs::LandTilePtr tile = neighborPair.second;
			const float value = (tile->terrain == terrainPreference ? 10.0f : 1.0f) * (tile->GetBiome() == biomePreference ? 10.0f : 1.0f) * tile->GetEnvironmentRating() * tile->GetMovementRating();

			if (value > bestNeighborValue)
			{
				bestNeighbor = neighborPair.second;
				bestNeighborValue = value;
			}
		}

		spread = true;
		bestNeighbor->SetCulture(culture);
		frontier.push_back(bestNeighbor);

		CleanFrontier();
	}

    return spread;
}

gs::CultureSpreader::CultureSpreader(const unsigned int speed, const gs::LandTilePtr origin, const shared_ptr<gs::Culture> value) :
	Spreader<shared_ptr<gs::LandTile>>(1, origin),
    culture(value),
    terrainPreference(origin->terrain),
	biomePreference(origin->GetBiome())
{
    origin->SetCulture(value);
    CleanFrontier();
}
