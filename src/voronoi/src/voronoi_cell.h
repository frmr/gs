#ifndef VORONOI_CELL_H
#define VORONOI_CELL_H

#include "../glm/glm.hpp"
#include <vector>
#include "mpreal/mpvec3.h"

class VoronoiCell
{
    public:

        VoronoiCell(mpvec3 p);

        mpvec3 position;

        std::vector<glm::dvec3> corners;

        void addCorner(glm::dvec3 c);
        void sortCorners();
        std::vector<glm::dvec3> quicksort(std::vector<glm::dvec3> points);

        glm::dvec3 centroid;
        void computeCentroid();
};

#endif