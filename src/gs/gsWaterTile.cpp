#include "gsLandTextureGenerator.hpp"
#include "gsBoundingBox.hpp"
#include "gsMath.hpp"
#include "gsWaterTile.hpp"


gs::WaterTile::WaterTile(const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid)
    :   gs::Tile(gs::Tile::Type::WATER, vertices, centroid, 0.0),
        ice(false)
{
    color = gs::Vec3d(0.09f, 0.13f, 0.33f);
}

void gs::WaterTile::GenerateTexture()
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

	const int width = std::max((int)(boundingBox.maxCoord.x * pixelsPerUnit), 1) + edgeCushion * 2;
	const int height = std::max((int)(boundingBox.maxCoord.y * pixelsPerUnit), 1) + edgeCushion * 2;

	texture = std::make_shared<gs::Texture>(width, height);

	texCoords.reserve(vertices.size());
	for (const auto& coord : relativeCoords)
	{
		texCoords.emplace_back(coord.x * pixelsPerUnit + edgeCushion, coord.y * pixelsPerUnit + edgeCushion);
	}

	const gs::Vec3d xJump = refAxisU / (double)pixelsPerUnit;
	const gs::Vec3d yJump = refAxisV / (double)pixelsPerUnit;

	const gs::Vec3d pixelOriginWorldCoord = vertices[0]->position - (xJump * (texCoords.front().x - edgeCushion)) - (yJump * (texCoords.front().y - edgeCushion)); //world coordinate of pixel (0,0)


	//sample texture generator at each pixel
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const gs::Vec3d pixelWorldCoord = (pixelOriginWorldCoord + xJump * (x - edgeCushion) + yJump * (y - edgeCushion)).Unit(); //TODO: Speed this up by using xJump and yJump to increment for each pixel

			texture->SetColor(x, y, waterTextureGenerator.Sample());
			//texture->SetColor(x, y, pixelWorldCoord * 255.0);
		}
	}
}
