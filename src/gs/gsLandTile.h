#ifndef GS_LAND_TILE_H
#define GS_LAND_TILE_H

#include "gsCulture.h"
#include "gsRandomRange.h"
#include "gsTexture.h"
#include "gsTile.h"
#include "gsVec2.h"
#include "gsVertex.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace gs
{
	class BiomeTextureGenerator;

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
        shared_ptr<gs::Culture>				culture;

		static gs::BiomeTextureGenerator	biomeTextureGenerator;

    private:
        Terrain                             DetermineTerrain() const;
		bool								CheckCoordIsNearCoast(const gs::Vec3d& coord) const;

    public:
        void                                GenerateTexture();
        Biome                               GetBiome() const;
        vector<shared_ptr<gs::LandTile>>    GetUnassignedBiomeNeighbors() const;

        bool                                HasUnassignedBiomeNeighbors() const;
        void                                SetBiome(const Biome newBiome);
        void                                SetCulture(const shared_ptr<Culture> newCulture);
        bool                                SpawnRiver(const int newRiverId, gs::RandomRange<double>& rand);

    public:
        LandTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid, const double height, const int regionId);
        ~LandTile();
    };

    typedef shared_ptr<gs::LandTile> LandTilePtr;
}

#endif // GS_LAND_TILE_H
