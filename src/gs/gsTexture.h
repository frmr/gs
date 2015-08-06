#ifndef GS_TILE_TEXTURE_H
#define GS_TILE_TEXTURE_H

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "gsArray.h"
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
    typedef gs::Vec3<int> Color;

    class Texture
    {
    private:
        int                 width;
        int                 height;

        gs::Array<GLubyte>  data;

    public:
        void        Blit( shared_ptr<Texture> source, const gs::Vec2i& offset );
        bool        CheckCoordIsValid( const int x, const int y ) const;
        int         GetArea() const;
        GLubyte     GetBlue( const int x, const int y );
        GLubyte*    GetData() const;
        GLubyte     GetGreen( const int x, const int y );
        int         GetHeight() const;
        GLubyte     GetRed( const int x, const int y );
        int         GetWidth() const;
        bool        SetBlue( const int x, const int y, const GLubyte blue );
        bool        SetColor( const int x, const int y, const GLubyte red, const GLubyte green, const GLubyte blue );
        //bool        SetColor( const int x, const int y, const gs::Vec3f& color );
        bool        SetColor( const int x, const int y, const gs::Color& color );
        bool        SetGreen( const int x, const int y, const GLubyte green );
        bool        SetRed( const int x, const int y, const GLubyte red );
        void        WriteToFile( const string& filename );

    public:
        Texture( const int width, const int height );
    };
}
#endif // GS_TILE_TEXTURE_H
