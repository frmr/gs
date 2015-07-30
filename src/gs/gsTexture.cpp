#include "gsTexture.h"
#include "gsMath.h"

#include <iostream>
#include <string>

using std::cerr;
using std::endl;

void gs::Texture::Blit( Texture* source, const gs::Vec2i& coord )
{
    for ( int sourceX = 0; sourceX < source->GetWidth(); ++sourceX )
    {
        for ( int sourceY = 0; sourceY < source->GetHeight(); ++sourceY )
        {
            int targetX = coord.x + sourceX;
            int targetY = coord.y + sourceY;
            if ( CheckCoordIsValid( targetX, targetY ) )
            {
                data.At( targetX, targetY, 0 ) = source->GetRed( sourceX, sourceY );
                data.At( targetX, targetY, 1 ) = source->GetGreen( sourceX, sourceY );
                data.At( targetX, targetY, 2 ) = source->GetBlue( sourceX, sourceY );
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
    cerr << "gs::Texture::Blit in src/gs/gsTexture.cpp - Invalid coordinate (" << x << "," << y << ") in texture with dimensions (" << width << "," << height << ")" << endl;
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

void gs::Texture::WriteToFile( const string filename )
{
    BMP image;
    image.SetSize( width, height);
    for ( int x = 0; x < width; ++x )
    {
        for ( int y = 0; y < height; ++y )
        {
            image(x,y)->Red = (int) GetRed( x, y );
            image(x,y)->Green = (int) GetGreen( x, y );
            image(x,y)->Blue = (int) GetBlue( x, y );
        }
    }
    image.WriteToFile( filename.c_str() );
}

gs::Texture::Texture( const int width, const int height )
    :   width( width ),
        height( height ),
        data( width, height, 3 )
{
}
