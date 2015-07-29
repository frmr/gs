#include "gsTexture.h"
#include "gsMath.h"

#include <iostream>
#include <string>

using std::cerr;
using std::endl;

void gs::Texture::Blit( Texture* source, const gs::Vec2i& coord )
{
    for ( int x = coord.x; x < source->GetWidth(); ++x )
    {
        for ( int y = coord.y; y < source->GetHeight(); ++y )
        {
            if ( CheckCoordIsValid( x, y ) )
            {
                data.At(x,y,0) = source->GetRed( x, y );
                data.At(x,y,1) = source->GetGreen( x, y );
                data.At(x,y,2) = source->GetBlue( x, y );
            }
        }
    }
}

bool gs::Texture::CheckCoordIsValid( const int x, const int y ) const
{
    if ( x >= 0 && x < width && y >= 0 && y < height )
    {
        return true;
    }
    cerr << "gs::Texture::Blit in src/gs/gsTexture.cpp - Cannot write coordinate (" << x << "," << y << ") in texture with dimensions (" << x << "," << y << ")" << endl;
    return false;
}

int gs::Texture::GetArea() const
{
    return width * height;
}

GLubyte gs::Texture::GetBlue( const int x, const int y )
{
    return CheckCoordIsValid( x, y ) ? data.At(x,y,2) : 0;
}

GLubyte gs::Texture::GetGreen( const int x, const int y )
{
    return CheckCoordIsValid( x, y ) ? data.At(x,y,1) : 0;
}

int gs::Texture::GetHeight() const
{
    return height;
}

GLubyte gs::Texture::GetRed( const int x, const int y )
{
    return CheckCoordIsValid( x, y ) ? data.At(x,y,0) : 0;
}

int gs::Texture::GetWidth() const
{
    return width;
}

bool gs::Texture::SetBlue( const int x, const int y, const GLubyte blue )
{
    if ( CheckCoordIsValid( x, y ) )
    {
        data.At(x,y,2) = blue;
        return true;
    }
    return false;
}

bool gs::Texture::SetColor( const int x, const int y, const GLubyte red, const GLubyte green, const GLubyte blue )
{
    if ( CheckCoordIsValid( x, y ) )
    {
        data.At(x,y,0) = red;
        data.At(x,y,1) = green;
        data.At(x,y,2) = blue;
        return true;
    }
    return false;
}

bool gs::Texture::SetColor( const int x, const int y, const gs::Vec3f& color )
{
    return SetColor( x, y, (GLubyte) ( color.x * 255.0f ), (GLubyte) ( color.y * 255.0f ), (GLubyte) ( color.z * 255.0f ) );
}

bool gs::Texture::SetGreen( const int x, const int y, const GLubyte green )
{
    if ( CheckCoordIsValid( x, y ) )
    {
        data.At(x,y,1) = green;
        return true;
    }
    return false;
}

bool gs::Texture::SetRed( const int x, const int y, const GLubyte red )
{
    if ( CheckCoordIsValid( x, y ) )
    {
        data.At(x,y,0) = red;
        return true;
    }
    return false;
}

gs::Texture::Texture( const int width, const int height )
    :   width( width ),
        height( height ),
        data( width, height, 3 )
{
}
