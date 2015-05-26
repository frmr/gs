#include "gsCamera.h"

#include <GL/gl3w.h>

#include <iostream>

using std::cerr;
using std::endl;

void gs::Camera::NormalizeLatitude()
{
    if ( latitude < -89.8f )
    {
        latitude = -89.9f;
    }
    else if ( latitude > 89.9f )
    {
        latitude = 89.9f;
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

void gs::Camera::ApplyTransformation( gs::MatrixStack<Matrix4>& modelView ) const
{
    modelView.top.identity();
    modelView.top.rotateY( -longitude );
    modelView.top.rotateX( -latitude );
    modelView.top.translate( position.GetX(), position.GetY(), position.GetZ() );
}

void gs::Camera::Move( const double latitudeChange, const double longitudeChange )
{
    latitude += latitudeChange;
    NormalizeLatitude();
    longitude += longitudeChange;
    NormalizeLongitude();

    position[0] = cos( longitude * 3.14159265f / 180.0f );
    position[1] = sin( longitude * 3.14159265f / 180.0f );
    position[2] = tan( latitude * 3.14159265f / 180.0f );

    position.Unit();
    position *= minZoomDistance + ( 1.0f - zoom ) * ( maxZoomDistance - minZoomDistance );

    cerr << latitude << " " << longitude << endl;
    cerr << position.GetX() << " " << position.GetY() << " " << position.GetZ() << endl;
}

void gs::Camera::Update( const InputState& input )
{
    //TODO: Zoom should affect movement speed
    float multiplier = ( ( input.GetUp() ^ input.GetDown() ) || ( input.GetLeft() ^ input.GetRight() ) ) ? 0.707f : 1.0f;

    float latitudeChange = 0.0f;
    float longitudeChange = 0.0f;

    if ( input.GetUp() )
    {
        latitudeChange += multiplier;
    }
    if ( input.GetDown() )
    {
        latitudeChange -= multiplier;
    }
    if ( input.GetLeft() )
    {
        longitudeChange -= multiplier;
    }
    if ( input.GetRight() )
    {
        longitudeChange += multiplier;
    }

    Move( latitudeChange, longitudeChange );
}

gs::Camera::Camera()
    :   latitude( 0.0f ),
        longitude( 0.0f ),
        zoom( 0.0f )
{
}
