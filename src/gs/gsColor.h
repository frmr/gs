#ifndef GS_COLOR_H
#define GS_COLOR_H

#include "gsRandomRange.h"
#include "gsVec3.h"

namespace gs
{
    class Color : public gs::Vec3<int>
    {
    private:
        static gs::RandomRange<int> randByte;

    public:
        Color();
        Color( const int r, const int g, const int b );
    };
}

#endif // GS_COLOR_H
