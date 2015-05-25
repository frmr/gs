#include "gsCamera.h"

#include <GL/gl3w.h>

void gs::Camera::NormalizeLatitude()
{
    if ( latitude < -90.0f )
    {
        latitude = -90.0f;
    }
    else if ( latitude > 90.0f )
    {
        latitude = 90.0f;
    }
}

void gs::Camera::NormalizeLongitude()
{
    if ( longitude < -180.0f )
    {
        longitude += 360.0f; //TODO: This could be more robust
    }
    else if ( longitude > 180.0f )
    {
        longitude -= 360.0f;
    }
}

void gs::Camera::ApplyTransformation( gs::MatrixStack<Matrix4>& matrix ) const
{
    matrix.top.identity();
    //matrix.top.rotateY( longitude );
    //matrix.top.rotateX( latitude );
    //matrix.top.translate( 0.0f, 0.0f, minZoomDistance + ( 1.0f - zoom ) * ( maxZoomDistance - minZoomDistance ) );
    matrix.top.translate( longitude, latitude, 0.0f );
}

void gs::Camera::Move( const double latitudeChange, const double longitudeChange )
{
    latitude += latitudeChange;
    NormalizeLatitude();
    longitude += longitudeChange;
    NormalizeLongitude();
}

void gs::Camera::Update( const InputState& input )
{
    //TODO: Zoom should affect movement speed
    float multiplier = ( ( input.GetUp() ^ input.GetDown() ) || ( input.GetLeft() ^ input.GetRight() ) ) ? 0.707f : 1.0f;
    if ( input.GetUp() )
    {
        latitude += multiplier;
    }
    if ( input.GetDown() )
    {
        latitude -= multiplier;
    }
    if ( input.GetLeft() )
    {
        longitude -= multiplier;
    }
    if ( input.GetRight() )
    {
        longitude += multiplier;
    }
}

gs::Camera::Camera()
    :   latitude( 0.0f ),
        longitude( 0.0f ),
        zoom( 0.0f ),
        x( 0.0 ),
        y( 0.0 ),
        z( maxZoomDistance )
{
}
