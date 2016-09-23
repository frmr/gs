#ifndef GS_TILE_TEXTURE_H
#define GS_TILE_TEXTURE_H

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "gsArray.h"
#include "gsColor.h"
#include "gsTile.h"
#include "gsLink.h"
#include "gsRandomRange.h" //TODO: delete
#include "gsVec2.h"
#include "gsVec3.h"
#include "gsVertex.h"

#include "../EasyBMP/EasyBMP.h"
#include "../gl3w/gl3w.h"

using std::shared_ptr;
using std::string;
using std::vector;

namespace gs
{
    class Texture
    {
    private:
        int                 width;
        int                 height;

        gs::Array<GLubyte>  data;

    public:
        void        Blit(const shared_ptr<const Texture>& source, const gs::Vec2i& offset);
        bool        CheckCoordIsValid(const int x, const int y) const;
		void		Delete();
		void		Fill(const gs::Color& color);
        int         GetArea() const;
        gs::Color   GetColor(const int x, const int y) const;
        GLubyte     GetBlue(const int x, const int y) const;
        GLubyte*    GetData() const;
        GLubyte     GetGreen(const int x, const int y) const;
        int         GetHeight() const;
        GLubyte     GetRed(const int x, const int y) const;
        int         GetWidth() const;
        GLuint      Push() const;
        bool        SetBlue(const int x, const int y, const GLubyte blue);
        bool        SetColor(const int x, const int y, const GLubyte red, const GLubyte green, const GLubyte blue);
        //bool        SetColor(const int x, const int y, const gs::Vec3d& color);
        bool        SetColor(const int x, const int y, const gs::Color& color);
        bool        SetGreen(const int x, const int y, const GLubyte green);
        bool        SetRed(const int x, const int y, const GLubyte red);
        void        WriteToFile(const string& filename) const;

    public:
        Texture(const int width, const int height);
		Texture(const int width, const int height, const gs::Color& color);
        explicit Texture(const string& filename);
    };
}
#endif // GS_TILE_TEXTURE_H
