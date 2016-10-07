#include "gsCultureSpreader.hpp"

#include <ctime>
#include <iostream>

using std::cerr;
using std::endl;

void gs::CultureSpreader::CleanFrontier()
{
    for (vector<gs::LandTilePtr>::iterator it = frontier.begin(); it != frontier.end();)
    {
        if (!(*it)->HasUnassignedBiomeNeighbors())
        {
            it = frontier.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

gs::LandTilePtr gs::CultureSpreader::SelectRandomTileFromVector(const vector<gs::LandTilePtr> tiles, gs::RandomRange<double>& randomIndex) const
{
    return (tiles.empty()) ? nullptr : tiles[(int) (randomIndex.Sample() * (double) tiles.size())];
}

bool gs::CultureSpreader::Spread()
{
//    for (unsigned int i = 0; i < speed; ++i)
//    {
//        if (frontier.empty())
//        {
//            return false;
//        }
//
//        vector<gs::LandTilePtr> unassignedNeighbors;
//
//        for (const auto& tile : frontier)
//        {
//            vector<gs::LandTilePtr> tileUnassignedNeighbors = tile->GetUnassignedBiomeNeighbors();
//            unassignedNeighbors.insert(unassignedNeighbors.end(), tileUnassignedNeighbors.begin(), tileUnassignedNeighbors.end());
//        }
//
//        vector<gs::LandTilePtr> unassignedPlains;
//        vector<gs::LandTilePtr> unassignedHills;
//        vector<gs::LandTilePtr> unassignedMountains;
//
//        for (const auto& tile : unassignedNeighbors)
//        {
//            if (tile->terrain == gs::LandTile::Terrain::PLAINS)
//            {
//                unassignedPlains.push_back(tile);
//            }
//            else if (tile->terrain == gs::LandTile::Terrain::HILLS)
//            {
//                unassignedHills.push_back(tile);
//            }
//            else if (tile->terrain == gs::LandTile::Terrain::MOUNTAINS)
//            {
//                unassignedMountains.push_back(tile);
//            }
//        }
//
//        //select best tile from the three vectors
//        gs::RandomRange<double> randomIndex(0.0, 0.9999, std::time(0));
//
//        gs::LandTilePtr plainsTile = SelectRandomTileFromVector(unassignedPlains, randomIndex);
//        gs::LandTilePtr hillsTile = SelectRandomTileFromVector(unassignedHills, randomIndex);
//        gs::LandTilePtr mountainsTile = SelectRandomTileFromVector(unassignedMountains, randomIndex);
//
//        gs::LandTilePtr bestTile = mountainsTile;
//
//        if (terrainPreference == gs::LandTile::Terrain::PLAINS)
//        {
//            bestTile = (hillsTile == nullptr) ? bestTile : hillsTile;
//            bestTile = (plainsTile == nullptr) ? bestTile : plainsTile;
//        }
//        else if (terrainPreference == gs::LandTile::Terrain::HILLS)
//        {
//            bestTile = (plainsTile == nullptr) ? bestTile : plainsTile;
//            bestTile = (hillsTile == nullptr) ? bestTile : hillsTile;
//        }
//
//        if (bestTile != nullptr) //TODO: prevent nullptr all together
//        {
//            bestTile->SetBiome(value);
//            frontier.push_back(bestTile);
//        }
//        else
//        {
//            return false;
//        }
//
//        CleanFrontier();
//
//    }

    return true;
}

gs::CultureSpreader::CultureSpreader(const unsigned int speed, const gs::LandTilePtr origin, const shared_ptr<gs::Culture> value, const gs::LandTile::Terrain terrainPreference)
    :   Spreader<shared_ptr<gs::LandTile>>(speed, origin),
        value(value),
        terrainPreference(terrainPreference)
{
    origin->SetCulture(value);
    CleanFrontier();
}
