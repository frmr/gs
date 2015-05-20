#ifndef GS_CONFIG_H
#define GS_CONFIG_H

namespace gs
{
    class Config
    {
    private:
        int     screenWidth;
        int     screenHeight;
        bool    fullscreen;
        
    public:
        int     GetScreenWidth() const;
        int     GetScreenHeight() const;
        bool    GetFullscreen() const;
        
        bool    SetScreenWidth( const int newScreenWidth );
        bool    SetScreenHeight( const int newScreenHeight );
        void    SetFullscreen( const bool newFullscreen );
        
    public:
        Config(const int screenWidth, const int screenHeight, const bool fullscreen);
    };
}

#endif