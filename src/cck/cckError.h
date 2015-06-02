#ifndef CCK_ERROR_H
#define CCK_ERROR_H

namespace cck
{
    enum class NodeError
    {
        SUCCESS,
        NEGATIVE_ID,
        ID_ALREADY_IN_USE,
        LATITUDE_OUT_OF_RANGE,
        LONGITUDE_OUT_OF_RANGE,
        NEGATIVE_RADIUS, //zero is also bad
        DIAMETER_EXCEEDS_SPHERE_CIRCUMFERENCE
    };

    enum class LinkError
    {
        SUCCESS,
        NEGATIVE_ID,
        DUPLICATE_ID,
        ID_NOT_FOUND,
        NODES_ALREADY_LINKED,
        TRIANGLE_CONFLICT
    };

    enum class NoiseError
    {
        SUCCESS,
        NON_POSITIVE_OCTAVES,
        NON_POSITIVE_PERSISTANCE,
        NON_POSITIVE_FREQUENCY
    };
}

#endif // CCK_ERROR_H
