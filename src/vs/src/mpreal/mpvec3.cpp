#include "mpvec3.h"

mpvec3::mpvec3() : x(0.0), y(0.0), z(0.0) {}
mpvec3::mpvec3(glm::dvec3 dvec) : x(dvec.x), y(dvec.y), z(dvec.z) {}
mpvec3::mpvec3(long double x, long double y, long double z) : x(x), y(y), z(z) {}

long double mpvec3::dot(mpvec3 v1, mpvec3 v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

mpvec3 mpvec3::cross(mpvec3 v1, mpvec3 v2)
{
    mpvec3 c;

    c.x = v1.y * v2.z - v1.z * v2.y;
    c.y = v1.z * v2.x - v1.x * v2.z;
    c.z = v1.x * v2.y - v1.y * v2.x;

    return c;
}

mpvec3 mpvec3::normalize(mpvec3 v)
{
    long double mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    v.x = v.x/mag;
    v.y = v.y/mag;
    v.z = v.z/mag;

    return v;
}

const mpvec3 mpvec3::operator-(const mpvec3 &other) const
{
    mpvec3 result = *this;
    result.x = result.x - other.x;
    result.y = result.y - other.y;
    result.z = result.z - other.z;
    return result;
}

glm::dvec3 mpvec3::to_dvec3()
{
    glm::dvec3 ret;
    ret.x = (long double)x;
    ret.y = (long double)y;
    ret.z = (long double)z;
    return ret;
}