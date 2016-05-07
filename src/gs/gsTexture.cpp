#include "gsTexture.h"
#include "gsMath.h"

#include "../lodepng/lodepng.h"

#include <iostream>
#include <string>

using std::cerr;
using std::endl;

void gs::Texture::Blit(const shared_ptr<const gs::Texture>& source, const gs::Vec2i& offset)
{
    for (int sourceX = 0; sourceX < source->GetWidth(); ++sourceX)
    {
        for (int sourceY = 0; sourceY < source->GetHeight(); ++sourceY)
        {
            const int targetX = offset.x + sourceX;
            const int targetY = offset.y + sourceY;

            #ifdef SAFE_TEXTURE
            if (CheckCoordIsValid(targetX, targetY))
            #endif
            {
                SetRed(targetX, targetY, source->GetRed(sourceX, sourceY));
                SetGreen(targetX, targetY, source->GetGreen(sourceX, sourceY));
                SetBlue(targetX, targetY, source->GetBlue(sourceX, sourceY));
            }
        }
    }
}

bool gs::Texture::CheckCoordIsValid(const int x, const int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return true;
    }
    cerr << "gs::Texture::Blit in src/gs/gsTexture.cpp - Invalid coordinate (" << x << "," << y << ") in texture with dimensions (" << width << "," << height << ")" << endl;
    return false;
}

void gs::Texture::Delete()
{
	data.Delete();
}

int gs::Texture::GetArea() const
{
    return width * height;
}

GLubyte gs::Texture::GetBlue(const int x, const int y) const
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        return data.GetAt(x,y,2);
    }
    return 0;
}

gs::Color gs::Texture::GetColor(const int x, const int y) const
{
    return gs::Color(data.GetAt(x, y, 0), data.GetAt(x, y, 1), data.GetAt(x, y, 2));
}

GLubyte* gs::Texture::GetData() const
{
    return data.GetData();
}

GLubyte gs::Texture::GetGreen(const int x, const int y) const
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        return data.GetAt(x,y,1);
    }
    return 0;
}

int gs::Texture::GetHeight() const
{
    return height;
}

GLubyte gs::Texture::GetRed(const int x, const int y) const
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        return data.GetAt(x,y,0);
    }
    return 0;
}

int gs::Texture::GetWidth() const
{
    return width;
}

GLuint gs::Texture::Push() const
{
    GLuint id;
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.GetData());

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    return id;
}

bool gs::Texture::SetBlue(const int x, const int y, const GLubyte blue)
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        data.At(x,y,2) = blue;
        return true;
    }
    return false;
}

bool gs::Texture::SetColor(const int x, const int y, const GLubyte red, const GLubyte green, const GLubyte blue)
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        data.At(x,y,0) = red;
        data.At(x,y,1) = green;
        data.At(x,y,2) = blue;
        return true;
    }
    return false;
}

//bool gs::Texture::SetColor(const int x, const int y, const gs::Vec3f& color)
//{
//    return SetColor(x, y, (GLubyte) (color.x * 255.0f), (GLubyte) (color.y * 255.0f), (GLubyte) (color.z * 255.0f));
//}

bool gs::Texture::SetColor(const int x, const int y, const gs::Color& color)
{
    return SetColor(x, y, color.x, color.y, color.z);
}

bool gs::Texture::SetGreen(const int x, const int y, const GLubyte green)
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        data.At(x,y,1) = green;
        return true;
    }
    return false;
}

bool gs::Texture::SetRed(const int x, const int y, const GLubyte red)
{
    #ifdef SAFE_TEXTURE
    if (CheckCoordIsValid(x, y))
    #endif
    {
        data.At(x,y,0) = red;
        return true;
    }
    return false;
}

void gs::Texture::WriteToFile(const string& filename) const
{
    BMP image;
    image.SetSize(width, height);
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            image(x,y)->Red = (int) GetRed(x, y);
            image(x,y)->Green = (int) GetGreen(x, y);
            image(x,y)->Blue = (int) GetBlue(x, y);
        }
    }
    image.WriteToFile(filename.c_str());
}

gs::Texture::Texture(const int width, const int height)
    :   width(width),
        height(height),
        data(width, height, 3)
{
}

gs::Texture::Texture(const string& filename)
    :   width(0),
        height(0),
        data(0)
{
    vector<unsigned char> image;
    unsigned imageWidth;
    unsigned imageHeight;
    lodepng::decode(image, imageWidth, imageHeight, filename);
    width = (int) imageWidth;
    height = (int) imageHeight;
    data.SetSize(width, height, 3);
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            data.At(x,y,0) = image[ 4 * y * width + 4 * x + 0 ];
            data.At(x,y,1) = image[ 4 * y * width + 4 * x + 1 ];
            data.At(x,y,2) = image[ 4 * y * width + 4 * x + 2 ];
        }
    }
}
