#include "mp_sample_generator.h"

#include <iostream>
#include <cstdlib>
#include <time.h>

MPSampleGenerator::MPSampleGenerator()
    :   seed(time(NULL)),
        re(seed)

{
    //std::cout << "seed = " << seed << "\n";

    unif = std::uniform_real_distribution<long double>(0.0,1.0);
}

MPSampleGenerator::MPSampleGenerator(unsigned int seed)
    :   seed(seed),
        re(seed)
{
    unif = std::uniform_real_distribution<long double>(0.0,1.0);
}

// Generates n * n jittered samples in the range [0,1) x [0,1)
mpvec3* MPSampleGenerator::getJitteredSamplesMP(int n)
{
    mpvec3* samples = new mpvec3[n*n];

    const long double minBound = 0.0;
    const long double maxBound = 1.0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            long double x = minBound + unif(re) * maxBound;
            long double y = minBound + unif(re) * maxBound;

            samples[i*n + j] = mpvec3((x+i)/n, (y+j)/n, 0.0);
        }
    }

    return samples;
}

// Generates n * n random samples in the range [0,1) x [0,1)
mpvec3* MPSampleGenerator::getRandomSamplesMP(int n)
{
    mpvec3* samples = new mpvec3[n*n];

    for (int i = 0; i < n*n; i++)
    {
        long double x = unif(re);
        long double y = unif(re);

        samples[i] = mpvec3(x, y, 0.0);
    }

    return samples;
}
