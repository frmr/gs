#ifndef VORONOI_GENERATOR_H
#define VORONOI_GENERATOR_H

#include "mp_sample_generator.h"
#include "voronoi_cell.h"
#include <vector>


class VoronoiGenerator
{
    public:

        VoronoiGenerator();
        VoronoiGenerator(unsigned int seed);
        ~VoronoiGenerator();

        void generateTessellation(int points);
        std::vector<VoronoiCell*> cell_vector;
    private:

        MPSampleGenerator sample_generator;

        void generatePoints(int points);
        void writeDataToFile();
};

#endif
