#ifndef MP_SAMPLE_GENERATOR_H
#define MP_SAMPLE_GENERATOR_H

#include "../glm/glm.hpp"

#include "mpreal/mpvec3.h"
#include <random>

class MPSampleGenerator
{
    public:

        MPSampleGenerator();
        MPSampleGenerator(unsigned int seed);

        mpvec3* getJitteredSamplesMP(int n);
        mpvec3* getRandomSamplesMP(int n);

    private:

        unsigned int seed;
        std::uniform_real_distribution<long double> unif;
        std::default_random_engine re;

};

#endif