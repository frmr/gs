#include "gsGlobe.hpp"

#include <ctime>
#include <iostream>
#include <thread>
#include <unordered_set>

#include "gsBiomeSpreader.hpp"
#include "gsCultureSpreader.hpp"
#include "gsLandTextureGenerator.hpp"
#include "gsRandomRange.hpp"
#include "gsSpreader.hpp"
#include "gsTexture.hpp"
#include "gsTileGroupManager.hpp"

using std::cerr;
using std::endl;

void gs::Globe::Draw(const gs::Camera& worldCamera) const
{
    Matrix4 inverseModelViewMatrix = worldCamera.GetViewMatrix();
    inverseModelViewMatrix.invert();

    //Draw land tiles
    landShader.Use();
    //pass matrices to shaders
    glUniformMatrix4fv(modelViewMatrixLocationLand, 1, false, worldCamera.GetViewMatrix().get()); //TODO: multiply by model matrix if needed
    glUniformMatrix4fv(projectionMatrixLocationLand, 1, false, worldCamera.GetProjectionMatrix().get());
    glUniformMatrix4fv(normalMatrixLocationLand, 1, false, inverseModelViewMatrix.getTranspose());
    landBuffer->Bind();
    groupManager.DrawLandTileGroups();

    //Draw water tiles
    waterShader.Use();
    //pass matrices to shaders
    glUniformMatrix4fv(modelViewMatrixLocationWater, 1, false, worldCamera.GetViewMatrix().get()); //TODO: multiply by model matrix if needed
    glUniformMatrix4fv(projectionMatrixLocationWater, 1, false, worldCamera.GetProjectionMatrix().get());
    glUniformMatrix4fv(normalMatrixLocationWater, 1, false, inverseModelViewMatrix.getTranspose());
    waterBuffer->Bind();
    groupManager.DrawWaterTileGroups();

    //glUseProgram(0);
}

cck::Globe gs::Globe::GenerateTerrain() const
{
    cerr << "Generating terrain" << endl;
    cck::Globe terrain(6370.0, (unsigned int) std::time(0));
    terrain.SetNoiseParameters(8, 0.75, 0.00015);

    terrain.AddNode(0,   52.0,   -4.0,   -0.2,   0.5,    600.0);    //Britain         GRASSLAND
    terrain.AddNode(1,   46.0,   2.0,    -0.2,   0.5,    600.0);    //France          GRASSLAND
    terrain.AddNode(2,   40.0,   -4.0,   -0.2,   1.0,    750.0);    //Iberia          SEMI_ARID
    terrain.AddNode(3,   50.0,   10.0,   -0.3,   0.75,   500.0);    //Germany         GRASSLAND
    terrain.AddNode(4,   42.0,   15.0,   -0.3,   0.75,   550.0);    //Italy           GRASSLAND
    terrain.AddNode(5,   65.0,   17.0,   -1.0,   1.5,    1000.0);   //Scandinavia     GRASSLAND
    terrain.AddNode(6,   50.0,   25.0,   -0.2,   0.5,    800.0);    //Poland          GRASSLAND
    terrain.AddNode(7,   40.0,   25.0,   -0.5,   1.5,    600.0);    //Balkans         GRASSLAND
    terrain.AddNode(8,   57.0,   44.0,   -0.3,   0.75,   1200.0);   //Russia          GRASSLAND
    terrain.AddNode(9,   38.0,   38.0,   -1.5,   3.0,    1000.0);   //Turkey          SEMI_ARID
    terrain.AddNode(10,  26.0,   44.0,   -0.5,   1.0,    1600.0);   //Arabia          DESERT
    terrain.AddNode(11,  35.0,   56.0,   -1.0,   2.0,    700.0);    //Persia          DESERT
    terrain.AddNode(12,  48.0,   68.0,   -0.2,   0.75,   1100.0);   //Central Asia    SEMI_ARID
    terrain.AddNode(13,  65.0,   75.0,   -0.2,   0.5,    1300.0);   //Siberia 1       GRASSLAND
    terrain.AddNode(14,  65.0,   106.0,  -0.3,   0.75,   1400.0);   //Siberia 2       GRASSLAND
    terrain.AddNode(15,  32.0,   88.0,   1.0,    4.0,    500.0);    //Himalayas       SEMI_ARID
    terrain.AddNode(29,  44.0,   95.0,   -1.0,   2.5,    800.0);    //Xinjiang        DESERT
    terrain.AddNode(16,  25.0,   80.0,   -0.4,   1.0,    1000.0);   //North India     GRASSLAND
    terrain.AddNode(17,  15.0,   77.0,   -0.4,   1.0,    1000.0);   //South India     SEMI_ARID
    terrain.AddNode(18,  25.0,   102.0,  -1.0,   2.0,    700.0);    //Burma           GRASSLAND
    terrain.AddNode(19,  14.0,   103.0,  -0.5,   0.75,   800.0);    //Indochina       GRASSLAND
    terrain.AddNode(20,  0.0,    103.0,  -0.4,   1.0,    850.0);    //Malaysia        GRASSLAND
    terrain.AddNode(21,  30.0,   107.0,  -0.5,   2.0,    400.0);    //Inner China     GRASSLAND
    terrain.AddNode(22,  23.0,   115.0,  -1.0,   1.5,    600.0);    //South China     GRASSLAND
    terrain.AddNode(23,  35.0,   115.0,  -0.3,   0.5,    400.0);    //North China     GRASSLAND
    terrain.AddNode(24,  47.0,   109.0,  -0.5,   1.5,    900.0);    //Mongolia        SEMI_ARID
    terrain.AddNode(25,  38.0,   127.0,  -0.5,   0.75,   350.0);    //Korea           GRASSLAND
    terrain.AddNode(26,  65.0,   144.0,  -0.5,   1.0,    800.0);    //Siberia 3       TUNDRA
    terrain.AddNode(27,  62.0,   165.0,  -1.0,   2.0,    1000.0);   //Kamchatka       TUNDRA
    terrain.AddNode(28,  36.0,   138.0,  -0.8,   2.0,    800.0);    //Japan           GRASSLAND

    //terrain.AddNode(50, 0.0, -90, 1.0, 1.0, 1000);    //Andes test above water
    //terrain.AddNode(51, 0.0, -110, -1.0, -1.0, 1000); //Andes test below water

    terrain.LinkNodes(1,     2,  -1.6,   4.0,    150.0,  50.0);     //France, Iberia
    terrain.LinkNodes(1,     3,  -0.3,   0.75,   150.0,  50.0);     //France, Germany
    terrain.LinkNodes(1,     4,  -0.3,   5.0,    150.0,  50.0);     //France, Italy
    terrain.LinkNodes(4,     2,  -0.3,   0.75,   150.0,  50.0);     //Italy, Iberia
    terrain.LinkNodes(4,     7,  -0.5,   0.75,   150.0,  50.0);     //Italy, Balkans
    terrain.LinkNodes(3,     4,  -0.3,   5.0,    150.0,  50.0);     //Germany, Italy
    terrain.LinkNodes(3,     5,  -0.75,  0.75,   150.0,  50.0);     //Germany, Scandinavia
    terrain.LinkNodes(3,     6,  -0.4,   0.5,    150.0,  50.0);     //Germany, Poland
    terrain.LinkNodes(3,     7,  -0.75,  1.5,    150.0,  50.0);     //Germany, Balkans
    terrain.LinkNodes(5,     8,  -1.0,   1.5,    150.0,  50.0);     //Scandinavia, Russia
    terrain.LinkNodes(5,     6,  -1.0,   1.5,    150.0,  50.0);     //Scandinavia, Poland
    terrain.LinkNodes(6,     8,  -0.5,   0.5,    150.0,  50.0);     //Poland, Russia
    terrain.LinkNodes(6,     7,  -1.0,   1.0,    150.0,  50.0);     //Poland, Balkans
    terrain.LinkNodes(7,     9,  -3.0,   3.0,    150.0,  50.0);     //Balkans, Turkey
    terrain.LinkNodes(8,     9,  -1.5,   4.0,    150.0,  50.0);     //Russia, Turkey
    terrain.LinkNodes(7,     8,  -1.0,   3.0,    150.0,  50.0);     //Balkans, Russia
    terrain.LinkNodes(9,     10, -3.0,   3.0,    150.0,  50.0);     //Turkey, Arabia
    terrain.LinkNodes(8,     12, -0.5,   0.75,   150.0,  50.0);     //Russia, Central Asia
    terrain.LinkNodes(8,     13, -0.3,   0.75,   150.0,  50.0);     //Russia, Siberia 1
    terrain.LinkNodes(12,    13, -0.25,  0.75,   150.0,  50.0);     //Central Asia, Siberia 1
    terrain.LinkNodes(9,     12, -0.2,   0.75,   150.0,  50.0);     //Turkey, Central Asia
    terrain.LinkNodes(9,     11, -3.0,   3.0,    150.0,  50.0);     //Turkey, Persia
    terrain.LinkNodes(11,    12, -2.0,   2.0,    150.0,  50.0);     //Persia, Central Asia
    terrain.LinkNodes(13,    14, -0.5,   0.75,   150.0,  50.0);     //Siberia 1, Siberia 2
    terrain.LinkNodes(11,    16, -2.0,   2.0,    150.0,  50.0);     //Persia, North India
    terrain.LinkNodes(16,    17, -0.5,   1.0,    150.0,  50.0);     //North India, South India
    terrain.LinkNodes(16,    15, -0.5,   1.0,    150.0,  50.0);     //North India, Himalayas
    terrain.LinkNodes(12,    16, -0.5,   1.0,    150.0,  50.0);     //Central Asia, North India
    terrain.LinkNodes(12,    15, -0.2,   0.75,   150.0,  50.0);     //Central Asia, Himalayas
    terrain.LinkNodes(16,    18, -0.5,   1.0,    150.0,  50.0);     //North India, Burma
    terrain.LinkNodes(18,    19, -1.0,   1.5,    150.0,  50.0);     //Burma, Indochina
    terrain.LinkNodes(19,    20, -0.5,   0.75,   150.0,  50.0);     //Indochina, Malaysia
    terrain.LinkNodes(15,    18, -1.0,   1.5,    150.0,  50.0);     //Himalayas, Burma
    terrain.LinkNodes(18,    21, -1.0,   1.5,    150.0,  50.0);     //Burma, Inner China
    terrain.LinkNodes(18,    22, -1.0,   1.5,    150.0,  50.0);     //Burma, South China
    terrain.LinkNodes(21,    22, -0.5,   2.0,    150.0,  50.0);     //Inner China, South China
    terrain.LinkNodes(21,    15, 1.0,    2.0,    150.0,  50.0);     //Inner China, Himalayas
    terrain.LinkNodes(22,    23, -0.5,   1.0,    150.0,  50.0);     //South China, North China
    terrain.LinkNodes(21,    23, -0.5,   2.0,    150.0,  50.0);     //Inner China, North China
    terrain.LinkNodes(23,    24, -0.3,   0.5,    150.0,  50.0);     //North China, Mongolia
    terrain.LinkNodes(21,    24, -0.5,   1.5,    150.0,  50.0);     //Inner China, Mongolia
    terrain.LinkNodes(14,    24, -0.5,   1.5,    150.0,  50.0);     //Siberia 2, Mongolia
    terrain.LinkNodes(14,    26, -0.5,   1.0,    150.0,  50.0);     //Siberia 2, Siberia 3
    terrain.LinkNodes(24,    26, -0.5,   1.0,    150.0,  50.0);     //Mongolia, Siberia 3
    terrain.LinkNodes(24,    25, -0.5,   1.5,    150.0,  50.0);     //Mongolia, Korea
    terrain.LinkNodes(25,    26, -0.5,   1.0,    150.0,  50.0);     //Korea, Siberia 3
    terrain.LinkNodes(26,    27, -0.5,   1.0,    150.0,  50.0);     //Siberia 3, Siberia 4

    terrain.LinkNodes(12,    29, -0.2,   0.75,   150.0,  50.0);     //Central Asia, Xinjiang
    terrain.LinkNodes(13,    29, -0.2,   0.5,    150.0,  50.0);     //Siberia 1, Xinjiang
    terrain.LinkNodes(14,    29, -0.3,   0.75,   150.0,  50.0);     //Siberia 2, Xinjiang
    terrain.LinkNodes(24,    29, -0.5,   1.5,    150.0,  50.0);     //Mongolia, Xinjiang
    terrain.LinkNodes(15,    29, 0.0,    2.5,    150.0,  50.0);     //Himalayas, Xinjiang
    terrain.LinkNodes(21,    29, -0.5,   2.0,    150.0,  50.0);     //Inner, Xinjiang

    //terrain.LinkNodes(50, 51, 1.5, 3.0, 200.0, 50.0f); //Andes test

    cerr << "Generated terrain" << endl;

    return terrain;
}

void gs::Globe::PrintMeshProperties() const
{
    cerr << "Globe vertices: " << vertices.size() << endl;
    cerr << "Globe edges: " << edges.size() << endl;
    cerr << "Globe land tiles: " << landTiles.size() << endl;
    cerr << "Globe water tiles: " << waterTiles.size() << endl;
    cerr << "Globe total tiles: " << allTiles.size() << endl;

    for (auto edge : edges)
    {
        if (edge->GetTiles().size() != 2)
        {
            cerr << "gs::Globe::GenerateTiles() in gsGlobe.cpp: Edge has " << edge->GetTiles().size() << " adjacent face." << endl;
        }
    }
}

void gs::Globe::Update()
{

}

void gs::Globe::AssignBufferOffsets()
{
    GLuint bufferOffset = 0;

    for (auto& tile : landTiles)
    {
        bufferOffset = tile->SetBufferOffset(bufferOffset);
    }

    bufferOffset = 0;

    for (auto& tile : waterTiles)
    {
        bufferOffset = tile->SetBufferOffset(bufferOffset);
    }
}

void gs::Globe::BuildBiomeTable()
{
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Britain
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //France
    biomeTable.push_back(gs::LandTile::Biome::SEMI_ARID); //Iberia
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Germany
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Italy
    biomeTable.push_back(gs::LandTile::Biome::TUNDRA);    //Scandinavia
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Poland
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Balkans
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Russia
    biomeTable.push_back(gs::LandTile::Biome::SEMI_ARID); //Turkey
    biomeTable.push_back(gs::LandTile::Biome::DESERT);    //Arabia
    biomeTable.push_back(gs::LandTile::Biome::DESERT);    //Persia
    biomeTable.push_back(gs::LandTile::Biome::SEMI_ARID); //Central Asia
    biomeTable.push_back(gs::LandTile::Biome::TUNDRA); //Siberia 1
    biomeTable.push_back(gs::LandTile::Biome::TUNDRA); //Siberia 2
    biomeTable.push_back(gs::LandTile::Biome::SEMI_ARID); //Himalayas
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //North India
    biomeTable.push_back(gs::LandTile::Biome::SEMI_ARID); //South India
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Burma
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Indochina
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Malaysia
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Inner China
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //South China
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //North China
    biomeTable.push_back(gs::LandTile::Biome::SEMI_ARID); //Mongolia
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Korea
    biomeTable.push_back(gs::LandTile::Biome::TUNDRA);    //Siberia 3
    biomeTable.push_back(gs::LandTile::Biome::TUNDRA);    //Kamchatka
    biomeTable.push_back(gs::LandTile::Biome::GRASSLAND); //Japan
    biomeTable.push_back(gs::LandTile::Biome::DESERT);    //Xinjiang
}

void gs::Globe::CombineVertices(const vector<glm::dvec3>& corners, gs::Array<vector<gs::VertexPtr>>& buckets, const unsigned int bucketDim, vector<gs::VertexPtr>& cellVertices)
{
    constexpr double errorMargin = 0.0000001;
    for (const auto& corner : corners)
    {
        const gs::Vec3d gsCorner((float) corner.x, (float) corner.y, (float) corner.z);
        //hash to buckets
        unsigned int xHash = HashDouble(gsCorner.x, bucketDim);
        unsigned int yHash = HashDouble(gsCorner.y, bucketDim);
        unsigned int zHash = HashDouble(gsCorner.z, bucketDim);

        bool foundVertex = false;

        for (unsigned int xi = ((xHash == 0) ? xHash : xHash - 1); xi <= ((xHash == bucketDim - 1) ? xHash : xHash + 1); ++xi)
        {
            for (unsigned int yi = ((yHash == 0) ? yHash : yHash - 1); yi <= ((yHash == bucketDim - 1) ? yHash : yHash + 1); ++yi)
            {
                for (unsigned int zi = ((zHash == 0) ? zHash : zHash - 1); zi <= ((zHash == bucketDim - 1) ? zHash : zHash + 1); ++zi)
                {
                    for (auto v : buckets.At(xi, yi, zi))
                    {
                        if (v->position.x < gsCorner.x + errorMargin && v->position.x > gsCorner.x - errorMargin &&
                             v->position.y < gsCorner.y + errorMargin && v->position.y > gsCorner.y - errorMargin &&
                             v->position.z < gsCorner.z + errorMargin && v->position.z > gsCorner.z - errorMargin)
                        {
                            cellVertices.push_back(v);
                            foundVertex = true;
                            break;
                        }
                    }
                }
                if (foundVertex)
                {
                    break;
                }
            }
            if (foundVertex)
            {
                break;
            }
        }

        if (!foundVertex)
        {
            auto newVertex = std::make_shared<gs::Vertex>(gsCorner);
            cellVertices.push_back(newVertex);
            vertices.push_back(newVertex);
            buckets.At(xHash, yHash, zHash).push_back(newVertex);
        }
    }
}

void gs::Globe::CreateTile(const vector<gs::VertexPtr>& cellVertices, const cck::Globe& terrain, const cck::Vec3& centroid)
{
    //create new tile
    double sampleHeight;
    int sampleId;
    //terrain.SampleData(centroid.ToGeographic(), sampleHeight, sampleId);
    terrain.SampleData(cck::Vec3(centroid.z, centroid.x, centroid.y).ToGeographic(), sampleHeight, sampleId);

    gs::Vec3d gsCentroid(centroid.x, centroid.y, centroid.z);

    if (sampleHeight > 0.0)
    {
        auto newTile = std::make_shared<gs::LandTile>(cellVertices, gsCentroid, sampleHeight, sampleId);
        allTiles.push_back(std::dynamic_pointer_cast<gs::Tile>(newTile));
        landTiles.push_back(newTile);

        //add tile to all cell vertices
        for (auto& v : cellVertices)
        {
            v->AddTile(std::dynamic_pointer_cast<gs::Tile>(newTile));
        }
    }
    else
    {
        auto newTile = std::make_shared<gs::WaterTile>(cellVertices, gsCentroid);
        allTiles.push_back(std::dynamic_pointer_cast<gs::Tile>(newTile));
        waterTiles.push_back(newTile);

        //add tile to all cell vertices
        for (auto& v : cellVertices)
        {
            v->AddTile(std::dynamic_pointer_cast<gs::Tile>(newTile));
        }
    }
}

void gs::Globe::CreateTileEdges(const vector<gs::VertexPtr>& cellVertices)
{
    for (unsigned int i = 0; i < cellVertices.size(); ++i)
    {
        auto v0 = cellVertices[i];
        auto v1 = cellVertices[(i == cellVertices.size() - 1) ? 0 : i + 1];

        auto edge = v0->GetEdgeWith(v1);

        if (edge == nullptr)
        {
            auto newEdge = std::make_shared<gs::Edge>(v0, v1);
            newEdge->AddTile(allTiles.back());

            v0->AddLink(gs::Link<gs::Vertex>(v1, newEdge));
            v1->AddLink(gs::Link<gs::Vertex>(v0, newEdge));

            edges.push_back(newEdge);
        }
        else
        {
            edge->AddTile(allTiles.back());

            //link tiles on each side of the edge to each other
            vector<gs::TilePtr> edgeTiles = edge->GetTiles();

            LinkTiles(edgeTiles.front(), edgeTiles.back(), edge);
            LinkTiles(edgeTiles.back(), edgeTiles.front(), edge);
        }
    }
}

//val must be between 0 and 1
unsigned int gs::Globe::HashDouble(const double val, const int bucketDim)
{
    int hashVal = (((unsigned int) (val / 1.0 * (double) bucketDim)) + bucketDim) / 2;
    if (hashVal > bucketDim - 1)
    {
        hashVal = bucketDim - 1;
    }
    else if (hashVal < 0)
    {
        hashVal = 0;
    }
    return (unsigned int) hashVal;
}

void gs::Globe::GenerateBiomes(const int numOfSpreaders)
{
    cerr << "Generating biomes" << endl;

    if (landTiles.empty())
    {
        cerr << "gs::Globe::GenerateBiomes() in gsGlobe.cpp: Cannot spawn biome spreaders because there are no land tiles." << endl;
        return;
    }

    /*
    gs::RandomRange<double> randomIndex(0.0, 0.9999, std::time(0));

    //put numOfSpreaders random land tiles at the start of the vector
    for (unsigned int i = 0; i < (unsigned int) numOfSpreaders && i < landTiles.size(); ++i)
    {
        //swap current tile with random tile further along in the vector
        unsigned int index = i + (int) (randomIndex.Sample() * (double) (landTiles.size() - i));
        gs::LandTilePtr temp = landTiles[index];
        landTiles[index] = landTiles[i];
        landTiles[i] = temp;
    }

    //generate spreaders
    vector<gs::BiomeSpreader> spreaders;
    gs::RandomRange<int> randomSpeed(1, 4, std::time(0));

    for (unsigned int i = 0; i < (unsigned int) numOfSpreaders && i < landTiles.size(); ++i)
    {
        spreaders.push_back(gs::BiomeSpreader(randomSpeed.Sample(), landTiles[i], LookupRegionBiome(landTiles[i]->regionId), landTiles[i]->terrain));
    }

    //update each spreader until there is no more room to expand
    bool active = true;
    while (active)
    {
        active = false;
        for (auto& s : spreaders)
        {
            if (s.Spread() == true)
            {
                active = true;
            }
        }
    }
    */
    //assign biomes to tiles that the spreaders didn't reach
    for (auto& tile : landTiles)
    {
        if (tile->GetBiome() == gs::LandTile::Biome::UNASSIGNED)
        {
            tile->SetBiome(LookupRegionBiome(tile->regionId));
        }
    }
    cerr << "Generated biomes" << endl;
}

void gs::Globe::GenerateCultures(const int numOfCultures)
{
	assert(size_t(numOfCultures) < landTiles.size());

	for (auto& tile : landTiles)
	{
		tile->CalculateEnvironmentRating();
		tile->CalculateMovementRating();
	}

	cultures.reserve(numOfCultures);
	for (uint16_t i = 0; i < numOfCultures; ++i)
	{
		cultures.push_back(std::make_shared<gs::Culture>(i, i));
	}

	std::vector<gs::CultureSpreader> spreaders;

	std::unordered_set<int> originIds;

	gs::RandomRange<size_t> randomIndex(0, int(landTiles.size() - 1));

	for (const auto& culture : cultures)
	{
		gs::LandTilePtr origin = nullptr;

		do
		{
			origin = *(landTiles.begin() + randomIndex.Sample());
		} while (originIds.count(origin->id) > 0);
		
		spreaders.push_back(gs::CultureSpreader(1, origin, culture));
	}

	bool spreading = true;

	while (spreading)
	{
		spreading = false;

		for (auto& spreader : spreaders)
		{
			const bool spread = spreader.Spread();
			if (spread)
			{
				spreading = true;
			}
		}
	}
}

void gs::Globe::GenerateLandTextures()
{
	cerr << "Generating land textures" << endl;

	constexpr size_t numThreads = 4;
	const size_t tilesPerThread = landTiles.size() / numThreads;
	auto& tilesRef = landTiles;

	if (tilesPerThread >= 1)
	{
		std::vector<std::thread> threads;

		for (size_t threadIndex = 0; threadIndex < numThreads; ++threadIndex)
		{
			threads.push_back(std::thread([=, &tilesRef]() {
				for (size_t tileIndex = threadIndex * tilesPerThread; tileIndex < ((threadIndex == numThreads - 1) ? landTiles.size() : (threadIndex + 1) * tilesPerThread); ++tileIndex)
				{
					landTiles[tileIndex]->GenerateTexture();
				};
			}));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}
	else
	{
		for (auto& tile : landTiles)
		{
			tile->GenerateTexture();
		}
	}

	for (auto& tile : landTiles)
	{
		groupManager.Add(tile);
		tile->DeleteLocalTextureData();
		landBuffer->UpdateTexCoordBuffer(tile);
	}

	groupManager.PushLastLandTexture();
	cerr << "Generated land textures" << endl;
}

void gs::Globe::GenerateRivers(const int minRivers, const int maxRivers)
{
    cerr << "Generating rivers" << endl;
    //pick numOfRivers random vertices

    gs::RandomRange<double> rand(0.0, 0.9999, (unsigned int) std::time(0));

    for (int i = 0; i < maxRivers && i < int(landTiles.size());)
    {
        int swapTarget = i + (int) (rand.Sample() * (double) (landTiles.size() - i));
        gs::LandTilePtr temp = landTiles[swapTarget];
        landTiles[swapTarget] = landTiles[i];
        landTiles[i] = temp;

		if (landTiles[i]->SpawnRiver(i, rand) || i > minRivers)
		{
			++i;
		}
    }
    cerr << "Generated rivers" << endl;
}

void gs::Globe::GenerateWaterTextures()
{
	cerr << "Generating water textures" << endl;

	constexpr size_t numThreads = 4;
	const size_t tilesPerThread = waterTiles.size() / numThreads;
	auto& tilesRef = waterTiles;

	if (tilesPerThread >= 1)
	{
		std::vector<std::thread> threads;

		for (size_t threadIndex = 0; threadIndex < numThreads; ++threadIndex)
		{
			threads.push_back(std::thread([=, &tilesRef]() {
				for (size_t tileIndex = threadIndex * tilesPerThread; tileIndex < ((threadIndex == numThreads - 1) ? waterTiles.size() : (threadIndex + 1) * tilesPerThread); ++tileIndex)
				{
					waterTiles[tileIndex]->GenerateTexture();
				};
			}));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}
	else
	{
		for (auto& tile : waterTiles)
		{
			tile->GenerateTexture();
		}
	}

	for (auto& tile : waterTiles)
	{
		groupManager.Add(tile);
		tile->DeleteLocalTextureData();
		waterBuffer->UpdateTexCoordBuffer(tile);
	}

	groupManager.PushLastWaterTexture();
    cerr << "Generated water textures" << endl;
}

void gs::Globe::GenerateTiles(const int numOfTiles)
{
    cerr << "Generating tiles" << endl;
    VoronoiGenerator vg((unsigned int) std::time(0));
    vg.generateTessellation(numOfTiles);

    //generate world
    cck::Globe terrain = GenerateTerrain();

    //count vertices and initialise tiles
    //int vertexCount = 0;
    allTiles.reserve(numOfTiles);

    constexpr unsigned int bucketDim = 256;

    gs::Array<vector<gs::VertexPtr>> buckets(bucketDim, bucketDim, bucketDim);

    for (const auto& cell : vg.cell_vector)
    {
        vector<gs::VertexPtr> cellVertices;
        CombineVertices(cell->corners, buckets, bucketDim, cellVertices);
        CreateTile(cellVertices, terrain, cck::Vec3(cell->centroid.x, cell->centroid.y, cell->centroid.z));
        CreateTileEdges(cellVertices);
    }

    buckets.Delete();

    for (auto& edge : edges)
    {
        edge->Widen();
    }

    PrintMeshProperties();

    cerr << "Generated tiles" << endl;
}

void gs::Globe::LinkTiles(const gs::TilePtr source, const gs::TilePtr dest, const gs::EdgePtr edge)
{
    source->AddLink(gs::Link<gs::Tile>(dest, edge));
    if (dest->GetSurface() == gs::Tile::Type::LAND)
    {
        source->AddLink(gs::Link<gs::LandTile>(std::dynamic_pointer_cast<gs::LandTile>(dest), edge));
    }
    else if (dest->GetSurface() == gs::Tile::Type::WATER)
    {
        source->AddLink(gs::Link<gs::WaterTile>(std::dynamic_pointer_cast<gs::WaterTile>(dest), edge));
    }
    else
    {
        cerr << "gs::Globe::GenerateTiles() in gsGlobe.cpp: Face adjacent to edge has unassigned type." << endl;
    }
}

gs::LandTile::Biome gs::Globe::LookupRegionBiome(const int id) const
{
    return biomeTable[id];
}

void gs::Globe::SetTileGroupTextureSize()
{
    GLint maxSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
    maxSize = 2048; //TODO: Change this with config file
    groupManager.SetTextureSize(maxSize);
}

gs::Globe::Globe()
    :   landShader("land", "data/shaders/land.vert", "data/shaders/land.frag"),
        waterShader("water", "data/shaders/water.vert", "data/shaders/water.frag")
{
    //generate voronoi sphere
    const int numOfTiles = 16000;
    GenerateTiles(numOfTiles);

    //build biome table
    BuildBiomeTable();

    //Calculate vertex heights
    for (auto& v : vertices)
    {
        v->CalculateHeight();
    }

    //Generate planet
    GenerateRivers(10, 50);
    GenerateBiomes(200);
    GenerateCultures(20);
    //GenerateStates(150);

    AssignBufferOffsets();

    SetTileGroupTextureSize();






    //Allocate memory for land buffer and initialise tile data
    landBuffer = std::make_shared<gs::TileBuffer<gs::LandTile>>(landTiles, landShader);
    landBuffer->Bind();

    GenerateLandTextures();

    landShader.SetFragOutput("colorOut");
    landShader.Link();

    modelViewMatrixLocationLand = landShader.GetUniformLocation("modelViewMatrix");
    projectionMatrixLocationLand = landShader.GetUniformLocation("projectionMatrix");
    normalMatrixLocationLand = landShader.GetUniformLocation("normalMatrix");







    //Allocate memory for water buffer and initialise tile data
    waterBuffer = std::make_shared<gs::TileBuffer<gs::WaterTile>>(waterTiles, waterShader);
    waterBuffer->Bind();

	GenerateWaterTextures();

    waterShader.SetFragOutput("colorOut");
    waterShader.Link();

    modelViewMatrixLocationWater = waterShader.GetUniformLocation("modelViewMatrix");
    projectionMatrixLocationWater = waterShader.GetUniformLocation("projectionMatrix");
    normalMatrixLocationWater = waterShader.GetUniformLocation("normalMatrix");
}
