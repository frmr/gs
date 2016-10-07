#pragma once

#include <vector>

#include "../gl3w/gl3w.h"
#include "../EasyBMP/EasyBMP.h"
#include "gsTexture.h"

#include "gsTile.h"

using std::vector;

namespace gs
{
    class TileGroup
    {
	private:
		const int               textureSize;
		GLuint                  textureId;
		gs::Vec2i               shelfCursor;
		int                     shelfTop;
		shared_ptr<gs::Texture> texture;    //TODO: should be unique_ptr

	protected:
        const GLuint        bufferBegin;
        GLuint              bufferEnd;

    public:
		bool		Add(const gs::TilePtr& tile);
		void		DeleteLocalTextureData();
		void		PushTexture();
		void		WriteToFile(const std::string& filename = "") const;
        void		Draw() const;
        GLuint      GetBufferBegin() const;
        GLuint      GetBufferEnd() const;

    public:
        explicit TileGroup(const GLuint bufferBegin, const int textureSize);
		~TileGroup();
    };
}