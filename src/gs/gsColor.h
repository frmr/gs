#ifndef GS_COLOR_H
#define GS_COLOR_H

#include "gsRandomRange.h"
#include "gsVec3.h"
#include <cstdint>

namespace gs
{
    class Color : public gs::Vec3<uint8_t>
    {
    public:
        uint8_t& r;
        uint8_t& g;
        uint8_t& b;

    private:
        static gs::RandomRange<uint8_t> randByte;

    public:
        Color();
        Color(const uint8_t r, const uint8_t g, const uint8_t b);
		Color(const gs::Color& rhs);
		Color(const gs::Vec3i& rhs);
		Color(const gs::Vec3f& rhs);
    };
}

#endif // GS_COLOR_H
