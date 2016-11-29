#pragma once

#include <cmath>
#include <random>
#include <ctime>

namespace gs
{
    template<typename T>
    class RandomRange
    {
    private:
        std::default_random_engine              engine;
        std::uniform_real_distribution<double>  dist;
        bool                                    round;
    public:
        T Sample()
        {
            return (round) ? (T) std::round(dist(engine)) : (T) dist(engine);
        }
    public:
        RandomRange(const double min, const double max, const int seed = std::time(0))
            :   engine(seed),
                dist(min, max),
                round(false)
        {
        }

        RandomRange(const int min, const int max, const int seed = std::time(0))
            :   engine(seed),
                dist((double) min, (double) max),
                round(true)
        {
        }
    };
}