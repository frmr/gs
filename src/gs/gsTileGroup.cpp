#include "gsTileGroup.hpp"

#include <iostream>

using std::cerr;
using std::endl;

bool gs::TileGroup::Add(const gs::TilePtr& tile)
{
	//TODO: Improve bin packing
	//if tile texture doesn't fit on current shelf
	if (tile->GetTexture()->GetWidth() >= textureSize - shelfCursor.x)
	{
		//create new shelf
		shelfCursor.x = 0;
		shelfCursor.y = shelfTop;
	}

	//if tile texture is taller than current shelf
	if (tile->GetTexture()->GetHeight() > shelfTop - shelfCursor.y)
	{
		//raise the shelf
		shelfTop = shelfCursor.y + tile->GetTexture()->GetHeight();
	}

	if (shelfTop > textureSize)
	{
		return false;
	}
	else
	{
		bufferEnd = tile->GetIndexBufferEnd();
		tile->AddToTileGroupTexture(texture, shelfCursor, textureSize);
		shelfCursor.x += tile->GetTexture()->GetWidth();
		return true;
	}
}

void gs::TileGroup::DeleteLocalTextureData()
{
	texture.reset();
}

void gs::TileGroup::Draw() const
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glDrawElements(GL_TRIANGLES, (bufferEnd - bufferBegin) + 1, GL_UNSIGNED_INT, (void*)(bufferBegin * sizeof(GLuint))); //TODO: fix this memory leak (might be drivers?)
}

void gs::TileGroup::PushTexture()
{
	textureId = texture->Push();
}

void gs::TileGroup::WriteToFile(const std::string& filename) const
{
	if (filename == "")
	{
		texture->WriteToFile(("data/textures/procedural/" + std::to_string(bufferBegin) + ".bmp"));
	}
	else
	{
		texture->WriteToFile(filename);
	}
}

GLuint gs::TileGroup::GetBufferBegin() const
{
    return bufferBegin;
}

GLuint gs::TileGroup::GetBufferEnd() const
{
    return bufferEnd;
}

gs::TileGroup::TileGroup(const GLuint bufferBegin, const int textureSize) :
	bufferBegin(bufferBegin),
    bufferEnd(0),
	textureSize(textureSize),
	textureId(0),
	shelfCursor(0, 0),
	shelfTop(0)
		
{
	texture = std::make_shared<gs::Texture>(textureSize, textureSize);
    cerr << "New tile group at " << bufferBegin << endl;
}

gs::TileGroup::~TileGroup()
{
	DeleteLocalTextureData();
	//TODO: delete GL texture data?
}
