#include "gsCamera.h"
#include "gsMath.h"

#include "../gl3w/gl3w.h"

#include <iostream>

using std::cerr;
using std::endl;

void gs::Camera::NormalizeLatitude()
{
    if (latitude < -89.8f)
    {
        latitude = -89.9f;
    }
    else if (latitude > 89.9f)
    {
        latitude = 89.9f;
    }
}

void gs::Camera::NormalizeLongitude()
{
    if (longitude < -180.0f)
    {
        longitude += 360.0f; //TODO: This could be more robust
    }
    else if (longitude > 180.0f)
    {
        longitude -= 360.0f;
    }
}

Matrix4 gs::Camera::GetViewMatrix() const
{
    return viewMatrix;
}

Matrix4 gs::Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

void gs::Camera::Move(const float latitudeChange, const float longitudeChange)
{
    latitude += latitudeChange;
    NormalizeLatitude();
    longitude += longitudeChange;
    NormalizeLongitude();

    position[0] = cos(longitude * gs::PI / 180.0f);
    position[1] = tan(latitude * gs::PI / 180.0f);
    position[2] = sin(longitude * gs::PI / 180.0f);

    position.Unit();
    //position *= minZoomDistance + (1.0f - zoom) * (maxZoomDistance - minZoomDistance);

    UpdateViewMatrix();

    //cerr << latitude << " " << longitude << endl;
    //cerr << position.GetX() << " " << position.GetY() << " " << position.GetZ() << endl;
}

void gs::Camera::SetOrthographic(const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal)
{
    projectionMatrix[0] = 2 / (right - left);
    projectionMatrix[5]  = 2 / (top - bottom);
    projectionMatrix[10] = -2 / (farVal - nearVal);
    projectionMatrix[12] = -(right + left) / (right - left);
    projectionMatrix[13] = -(top + bottom) / (top - bottom);
    projectionMatrix[14] = -(farVal + nearVal) / (farVal - nearVal);
}

void gs::Camera::Update(InputState& input)
{
    //TODO: Zoom should affect movement speed

    const float multiplier = ((input.GetUp() ^ input.GetDown()) || (input.GetLeft() ^ input.GetRight())) ? 0.707f : 1.0f;

    float latitudeChange = 0.0f;
    float longitudeChange = 0.0f;

    if (input.GetUp())
    {
        latitudeChange += (input.GetShift()) ? 90.0f : multiplier;
    }
    if (input.GetDown())
    {
        latitudeChange -= (input.GetShift()) ? 90.0f : multiplier;
    }
    if (input.GetLeft())
    {
        longitudeChange -= (input.GetShift()) ? 90.0f : multiplier;
    }
    if (input.GetRight())
    {
        longitudeChange += (input.GetShift()) ? 90.0f : multiplier;
    }
    if (input.GetMouseWheelUp())
    {
        zoom -= 0.2f;
        if (zoom < minZoom)
        {
            zoom = minZoom;
        }
    }
    if (input.GetMouseWheelDown())
    {
        zoom += 0.2f;
        if (zoom > maxZoom)
        {
            zoom = maxZoom;
        }
    }

    SetOrthographic(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom, 0, 100);

    Move(latitudeChange, longitudeChange);
}

void gs::Camera::UpdateViewMatrix()
{
    viewMatrix.identity();
    viewMatrix.rotateY(-longitude);
    viewMatrix.rotateX(latitude);
    viewMatrix.translate(0.0f, 0.0f, -10.0f);
    //viewMatrix.translate(-position.GetX(), -position.GetY(), -position.GetZ());
}

gs::Camera::Camera(const float aspectRatio)
    :   aspectRatio(aspectRatio),
        latitude(0.0f),
        longitude(0.0f),
        zoom(2.0f)
{
}
