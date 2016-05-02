#include "voronoi_generator.h"

#include "voronoi.h"
#include <fstream>
#include <iostream>
#include <math.h>
//#include <sys/time.h>

VoronoiGenerator::VoronoiGenerator()
{

}

VoronoiGenerator::VoronoiGenerator(unsigned int seed) : sample_generator(seed)
{

}

VoronoiGenerator::~VoronoiGenerator()
{
    for (unsigned int i = 0; i < cell_vector.size(); i++)
    {
        delete cell_vector[i];
    }
}

void VoronoiGenerator::generateTessellation(int points)
{
    generatePoints(points);

    Voronoi voronoi;
    voronoi.construct(cell_vector);

    // sort corners of VoronoiCells
    for (unsigned int i = 0; i < cell_vector.size(); i++)
    {
        VoronoiCell* b = cell_vector[i];
        b->sortCorners();
        b->computeCentroid();
    }
}

// This method populates the cell_vector with VoronoiCells.
// Only the position of each VoronoiCell is set.
void VoronoiGenerator::generatePoints(int points)
{
    const long double PI = M_PIl;

    int c = sqrt(points);
    mpvec3* samples = sample_generator.getJitteredSamplesMP(c);

    for (int i = 0; i < c*c; i++)
    {
        mpvec3 sample = samples[i];

        // scale sample into range [0,2pi) x [-1,1]
        sample = mpvec3(2.0*PI*sample.x, 2.0*sample.y - 1.0, 0.0);

        long double T = sqrt(1.0 - sample.y * sample.y);
        long double x = T * cos(sample.x);
        long double y = T * sin(sample.x);
        long double z = sample.y;

        mpvec3 point = mpvec3::normalize(mpvec3(x,y,z));

        VoronoiCell* b = new VoronoiCell(point);
        cell_vector.push_back(b);
    }

    delete [] samples;
}

void VoronoiGenerator::writeDataToFile()
{
    std::ofstream file;
    file.open("output/voronoi_data", std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cout << "Unable to write data to file.\n";
        return;
    }

    for (unsigned int i = 0; i < cell_vector.size(); i++)
    {
        VoronoiCell* b = cell_vector[i];

        if (b->corners.size() > 2)
        {
            file << b->corners.size() << "\n";
            file << b->centroid.x << " ";
            file << b->centroid.y << " ";
            file << b->centroid.z << "\n";

            for (unsigned int c = 0; c < b->corners.size(); c++)
            {
                glm::dvec3 corner = b->corners[c];
                file << corner.x << " ";
                file << corner.y << " ";
                file << corner.z << "\n";
            }
        }
    }

    file.close();

    std::cout << "Data written to: output/voronoi_data\n";
}
