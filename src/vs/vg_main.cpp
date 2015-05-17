#include "src/voronoi_generator.h"
#include <iostream>

int main(int argc, char* argv[])
{
    int count = 10000; // default number of points
    if (argc == 2) count = atoi(argv[1]);
        
    VoronoiGenerator vg;
    vg.generateTessellation(count);

    return 0;
}
