#include "gsConfig.h"

int gs::Config::GetScreenWidth() const
{
    return screenWidth;
}

int gs::Config::GetScreenHeight() const
{
    return screenHeight;
}

bool gs::Config::GetFullscreen() const
{
    return fullscreen;
}

bool gs::Config::SetScreenWidth( const int newScreenWidth )
{
    if ( newScreenWidth <= 0 )
    {
        return false;
    }
    screenWidth = newScreenWidth;
    return true;
}

bool gs::Config::SetScreenHeight( const int newScreenHeight )
{
    if ( newScreenHeight <= 0 )
    {
        return false;
    }
    screenHeight = newScreenHeight;
    return true;
}

void gs::Config::SetFullscreen( const bool newFullscreen )
{
    fullscreen = newFullscreen;
}

gs::Config::Config( const int screenWidth, const int screenHeight, const bool fullscreen )
    :   screenWidth( screenWidth ),
        screenHeight( screenHeight ),
        fullscreen( fullscreen )
{
}
