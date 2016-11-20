#include "gsTile.hpp"

#include "gsLandTile.hpp"
#include "gsMath.hpp"
#include "gsWaterTile.hpp"

#include "gsLandTextureGenerator.hpp"

#include <iostream>
#include <limits>

using std::cerr;
using std::endl;

int gs::Tile::idCounter = 0;
gs::WaterTextureGenerator gs::Tile::waterTextureGenerator;

void gs::Tile::AddToTileGroupTexture(shared_ptr<gs::Texture> tileGroupTexture, const gs::Vec2i& tileGroupTextureOffset, const int tileGroupTextureSize)
{
	tileGroupTexture->Blit(texture, tileGroupTextureOffset);
	//update texture coordinates to be relative to the texture group texture
	for (unsigned int i = 0; i < texCoords.size(); ++i)
	{
		texCoords[i].x = ((double)tileGroupTextureOffset.x + texCoords[i].x) / (double)tileGroupTextureSize;
		texCoords[i].y = ((double)tileGroupTextureOffset.y + texCoords[i].y) / (double)tileGroupTextureSize;
	}
}

void gs::Tile::DeleteLocalTextureData()
{
	texture.reset();
}

shared_ptr<gs::Texture> gs::Tile::GetTexture() const
{
	return texture;
}

void gs::Tile::UpdateColorBuffer(const GLuint colorVbo) //TODO: const
{
    GLfloat* colorArray = new GLfloat[3*vertices.size()];
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        colorArray[3*i] = GLfloat(color.x);
        colorArray[3*i+1] = GLfloat(color.y);
        colorArray[3*i+2] = GLfloat(color.z);
    }
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 3 * bufferOffset * sizeof(GLfloat), 3 * vertices.size() * sizeof(GLfloat), colorArray);
    delete[] colorArray;
}

void gs::Tile::UpdateFogBuffer(const GLuint fogVbo) //TODO: const
{
    GLfloat* fogArray = new GLfloat[vertices.size()];
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        fogArray[i] = 0.0f;
    }
    glBindBuffer(GL_ARRAY_BUFFER, fogVbo);
    glBufferSubData(GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), fogArray);
    delete[] fogArray;
}

void gs::Tile::UpdatePositionBuffer(const GLuint positionVbo) //TODO: const
{
    GLfloat* positionArray = new GLfloat[3*vertices.size()];
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        positionArray[3*i] = (float) vertices[i]->position.x;
        positionArray[3*i+1] = (float) vertices[i]->position.y;
        positionArray[3*i+2] = (float) vertices[i]->position.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 3 * bufferOffset * sizeof(GLfloat), 3 * vertices.size() * sizeof(GLfloat), positionArray);
    delete[] positionArray;
}

void gs::Tile::AddVerticesToIndexVector(vector<GLuint>& indexVector)
{
    indexBufferOffset = GLuint(indexVector.size());
    indexVector.reserve(indexVector.size() + 3 * vertices.size());

    for (unsigned int i = 1; i < vertices.size() - 1; ++i)
    {
        indexVector.push_back(bufferOffset);
        indexVector.push_back(bufferOffset + i);
        indexVector.push_back(bufferOffset + i + 1);
    }
}

void gs::Tile::UpdateAllBuffers(const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo, const GLuint texCoordVbo) //const
{
	UpdatePositionBuffer(positionVbo);
	UpdateColorBuffer(colorVbo);
	UpdateFogBuffer(fogVbo);
	UpdateTexCoordBuffer(texCoordVbo);
}

void gs::Tile::UpdateTexCoordBuffer(const GLuint texCoordVbo)
{
	GLfloat* texCoordArray = new GLfloat[2 * vertices.size()];
	for (unsigned int i = 0; i < texCoords.size(); ++i)
	{
		texCoordArray[2 * i] = (GLfloat)texCoords[i].x;
		texCoordArray[2 * i + 1] = (GLfloat)texCoords[i].y;
	}
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * bufferOffset * sizeof(GLfloat), 2 * vertices.size() * sizeof(GLfloat), texCoordArray);
	delete[] texCoordArray;
}

bool gs::Tile::AddLink(const gs::Link<gs::Tile>& link)
{
    if (allLinks.size() == vertices.size())
    {
        return false;
    }
    allLinks.push_back(link);
    return true;
}

bool gs::Tile::AddLink(const gs::Link<gs::LandTile>& link)
{
    if (landLinks.size() == vertices.size())
    {
        return false;
    }
    landLinks.push_back(link);
    return true;
}

bool gs::Tile::AddLink(const gs::Link<gs::WaterTile>& link)
{
    if (waterLinks.size() == vertices.size())
    {
        return false;
    }
    waterLinks.push_back(link);
    return true;
}

GLuint gs::Tile::GetBufferEnd() const
{
    return bufferOffset + GLuint(vertices.size()) - 1;
}

GLuint  gs::Tile::GetBufferOffset() const
{
    return bufferOffset;
}

double gs::Tile::GetHeight() const
{
    return height;
}

GLuint gs::Tile::GetIndexBufferEnd() const
{
    return indexBufferOffset + (3 * (GLuint(vertices.size()) - 2)) - 1;
}

GLuint  gs::Tile::GetIndexBufferOffset() const
{
    return indexBufferOffset;
}

size_t  gs::Tile::GetNumOfVertices() const
{
    return vertices.size();
}

gs::Tile::Type gs::Tile::GetSurface() const
{
    return surface;
}

GLuint gs::Tile::SetBufferOffset(const GLuint vertexCount)
{
    bufferOffset = vertexCount;
    return vertexCount + GLuint(vertices.size());
}

gs::Tile::Tile(const Type surface, const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid, const double height) :
	id(idCounter++),
    surface(surface),
    normal(gs::Vec3d(centroid.x, centroid.y, centroid.z).Unit()),
    height(height),
    vertices(vertices),
    color(1.0f, 0.0f, 0.0f),
    fog(false),
	texture(nullptr)
{
}

gs::Tile::~Tile()
{
}
