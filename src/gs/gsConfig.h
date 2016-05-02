#ifndef GS_CONFIG_H
#define GS_CONFIG_H

#include <string>

using std::string;

namespace gs
{
    class Config
    {
    private:
        int     screenWidth;
        int     screenHeight;
        bool    fullscreen;

        string  textureDirectory;
        int     textureSize;
        int     texelsPerUnit;
        float   riverWidth;

    public:
        int     GetScreenWidth() const;
        int     GetScreenHeight() const;
        bool    GetFullscreen() const;

        bool    SetScreenWidth(const int newScreenWidth);
        bool    SetScreenHeight(const int newScreenHeight);
        void    SetFullscreen(const bool newFullscreen);

    public:
        Config();
        explicit Config(const string &filename);
    };
}

#endif
