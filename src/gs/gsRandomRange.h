#ifndef GS_RANDOM_RANGE_H
#define GS_RANDOM_RANGE_H

#include <random>

namespace gs
{
    class RandomRange
    {
    private:
        std::mt19937 mt;
        std::uniform_real_distribution<float> range;
    public:
        float Sample()
        {
            return range( mt );
        }
    public:
        RandomRange( const float min, const float max )
        {
            std::random_device rd;
            mt = std::mt19937(rd());
            range = std::uniform_real_distribution<float>( min, max );
        }
    };
}

#endif // GS_RANDOM_RANGE_H
