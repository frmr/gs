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

gs::Config::Config()
    :   screenWidth( 1024 ),
        screenHeight( 768 ),
        fullscreen( false )
{
}

gs::Config::Config( const string& filename )
    :   screenWidth( 1024 ),
        screenHeight( 768 ),
        fullscreen( false )
{
}
