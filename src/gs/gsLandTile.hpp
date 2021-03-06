#pragma once

#include "gsCulture.hpp"
#include "gsRandomRange.hpp"
#include "gsTexture.hpp"
#include "gsTile.hpp"
#include "gsUnit.hpp"
#include "gsVec2.hpp"
#include "gsVertex.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace gs
{
	class LandTextureGenerator;

    class LandTile : public gs::Tile
    {
    public:
        enum class Biome
        {
            UNASSIGNED = 0,
            ICE,
            TUNDRA,
            GRASSLAND,
            SEMI_ARID,
            DESERT
        };

        enum class Terrain
        {
            PLAINS = 0,
            HILLS,
            MOUNTAINS,
			LAKE
        };

    public:
        const int							regionId;
        const Terrain						terrain;

    private:
        bool								forested;
        Biome								biome;
		UnitF								environmentRating;
		UnitF								movementRating;
        shared_ptr<gs::Culture>				culture;
		

		static gs::LandTextureGenerator		landTextureGenerator;

    private:
		bool								CheckCoordIsNearCoast(const gs::Vec3d& coord) const;
        Terrain                             DetermineTerrain() const;
		UnitF								GetBiomeEnvironmentRating() const;
		UnitF								GetTerrainEnvironmentRating() const;
		UnitF								GetLatitudeEnvironmentRating() const;

    public:
		void								CalculateEnvironmentRating();
		void								CalculateMovementRating();
        void                                GenerateTexture();
        Biome                               GetBiome() const;
        vector<shared_ptr<gs::LandTile>>    GetUnassignedBiomeNeighbors() const;
		shared_ptr<gs::Culture>				GetCulture() const;
		float								GetEnvironmentRating() const;
		float								GetMovementRating() const;

        bool                                HasUnassignedBiomeNeighbors() const;
		bool								HasUnassignedCultureNeighbors() const;
		bool								HasDifferentCultureNeighbors() const;
        void                                SetBiome(const Biome newBiome);
        void                                SetCulture(const shared_ptr<Culture> newCulture);
        bool                                SpawnRiver(const int newRiverId, gs::RandomRange<double>& rand);

    public:
        LandTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid, const double height, const int regionId);
        ~LandTile();
    };

    typedef shared_ptr<gs::LandTile> LandTilePtr;
}