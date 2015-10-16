#include "gsColor.h"
#include <ctime>

gs::RandomRange<int> gs::Color::randByte( 0, 255, time( 0 ) );

gs::Color::Color()
    :   gs::Vec3<int>( randByte.Sample(), randByte.Sample(), randByte.Sample() ),
        r( x ),
        g( y ),
        b( z )
{
}

gs::Color::Color( const int r, const int g, const int b )
    :   gs::Vec3<int>( r, g, b ),
        r( x ),
        g( y ),
        b( z )
{
}
