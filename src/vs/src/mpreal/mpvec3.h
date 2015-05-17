#ifndef MPVEC3_H
#define MPVEC3_H

#include "../../glm/glm.hpp"

// This class implements 3 dimensional long double vectors

class mpvec3
{
    public:

        mpvec3();
        mpvec3(glm::dvec3 dvec);
        mpvec3(long double x, long double y, long double z);

        long double x, y, z;

        static long double dot(mpvec3 v1, mpvec3 v2);
        static mpvec3 cross(mpvec3 v1, mpvec3 v2);
        static mpvec3 normalize(mpvec3 v);

        const mpvec3 operator-(const mpvec3 &other) const;

        glm::dvec3 to_dvec3();
};

#endif