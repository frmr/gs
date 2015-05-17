#ifndef GS_GLOBE_H
#define GS_GLOBE_H

#include <vector>

using std::vector;

namespace gs
{
    class Globe
    {
        vector<gs::Vec3>    vertices;
        vector<gs::Tile>    tiles;
        vector<gs::Edge>    edges;
        Globe()
        {

        }
    };
}

#endif // GS_GLOBE_H
