#include "gsColor.h"
#include "gsMath.h"
#include <ctime>

gs::RandomRange<uint8_t> gs::Color::randByte(0, 255, time(0));

gs::Color::Color()
    :   gs::Vec3<uint8_t>(randByte.Sample(), randByte.Sample(), randByte.Sample()),
        r(x), g(y), b(z)
{
}

gs::Color::Color(const uint8_t r, const uint8_t g, const uint8_t b)
    :   gs::Vec3<uint8_t>(r, g, b),
        r(x), g(y), b(z)
{
}

gs::Color::Color(const gs::Color& rhs)
	: gs::Vec3<uint8_t>(rhs.x, rhs.y, rhs.z),
	  r(x), g(y), b(z)
{
}

gs::Color::Color(const gs::Vec3i& rhs)
	: gs::Vec3<uint8_t>((uint8_t) rhs.x, (uint8_t) rhs.y, (uint8_t) rhs.z),
	  r(x), g(y), b(z)
{
}

gs::Color::Color(const gs::Vec3d& rhs)
	: gs::Vec3<uint8_t>((uint8_t) gs::Clamp(rhs.x, 0.0, 255.0), (uint8_t) gs::Clamp(rhs.y, 0.0, 255.0), gs::Clamp(rhs.z, 0.0, 255.0)),
	  r(x), g(y), b(z)
{
}
