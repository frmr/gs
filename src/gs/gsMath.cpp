#include "gsMath.h"

template<typename T>
T gs::Dot( const gs::Vec2<T>& vecA, const gs::Vec2<T>& vecB )
{
    return vecA.x * vecB.y + vecA.y * vecB.y;
}

template<typename T>
T gs::Dot( const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB )
{
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

template<typename T>
gs::Vec3<T> gs::Cross( const gs::Vec3<T>& vecA, const gs::Vec3<T>& vecB )
{
    return gs::Vec3<T>( vecA.y * vecB.z - vecA.z * vecB.y,
                        vecA.z * vecB.x - vecA.x * vecB.z,
                        vecA.x * vecB.y - vecA.y * vecB.x );
}
