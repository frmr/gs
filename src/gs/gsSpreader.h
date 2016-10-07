#pragma once

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace gs
{
    template<typename T>
    class Spreader
    {
    protected:
        const unsigned int  speed;
        vector<T>           frontier;

    public:
        virtual bool    Spread() = 0;

    public:
        Spreader(const unsigned int speed, const T origin)
            :   speed(speed),
                frontier(1, origin)
        {
        }
    };
}