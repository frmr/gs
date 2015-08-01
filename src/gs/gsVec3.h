#ifndef GS_VEC3_H
#define GS_VEC3_H

#include <cmath>
#include "gsVec2.h"

namespace gs
{
    template<typename T>
    class Vec3
    {
    public:
        T   x, y, z;
        static constexpr T epsilon = (T) 0.000000;

    public:

        T Length() const
        {
            return sqrt( x * x + y * y + z * z );
        }

        Vec3<T>& Reverse()
        {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }

        Vec3<T>& Unit()
        {
            T length = Length();
            x /= length;
            y /= length;
            z /= length;
            return *this;
        }

        gs::Vec2<T> ToVec2()
        {
            return gs::Vec2<T>( x, y );
        }

        template<typename U>
        Vec3<T> operator+( const Vec3<U>& rhs ) const
        {
            return gs::Vec3<T>( x + (T) rhs.x, y + (T) rhs.y, z + (T) rhs.z );
        }

        template<typename U>
        Vec3<T> operator-( const Vec3<U>& rhs ) const
        {
            return gs::Vec3<T>( x - (T) rhs.x, y - (T) rhs.y, z - (T) rhs.z );
        }

        template<typename U>
        Vec3<T> operator*( const U& rhs ) const
        {
            return gs::Vec3<T>( x * (T) rhs, y * (T) rhs, z * (T) rhs );
        }

        template<typename U>
        Vec3<T> operator*( const Vec3<U>& rhs ) const
        {
            return gs::Vec3<T>( x * (T) rhs.x, y * (T) rhs.y, z * (T) rhs.z );
        }

        template<typename U>
        Vec3<T> operator/( const U& rhs ) const
        {
            return gs::Vec3<T>( x / (T) rhs, y / (T) rhs, z / (T) rhs );
        }

        template<typename U>
        Vec3<T>& operator+=( const Vec3<U>& rhs )
        {
            x += (T) rhs.x;
            y += (T) rhs.y;
            z += (T) rhs.z;
            return *this;
        }

        template<typename U>
        Vec3<T>& operator-=( const Vec3<U>& rhs )
        {
            x -= (T) rhs.x;
            y -= (T) rhs.y;
            z -= (T) rhs.z;
            return *this;
        }

        template<typename U>
        Vec3<T>& operator*=( const U& rhs )
        {
            x *= (T) rhs;
            y *= (T) rhs;
            z *= (T) rhs;
            return *this;
        }

        template<typename U>
        Vec3<T>& operator*=( const Vec3<U>& rhs )
        {
            x *= (T) rhs.x;
            y *= (T) rhs.y;
            z *= (T) rhs.z;
            return *this;
        }

        template<typename U>
        Vec3<T>& operator/=( const U& rhs )
        {
            x /= (T) rhs;
            y /= (T) rhs;
            z /= (T) rhs;
            return *this;
        }

        template<typename U>
        bool operator==( const U& rhs ) const
        {
            return ( x <= (T) rhs.x + epsilon &&
                     x >= (T) rhs.x - epsilon &&
                     y <= (T) rhs.y + epsilon &&
                     y >= (T) rhs.y - epsilon &&
                     z <= (T) rhs.z + epsilon &&
                     z >= (T) rhs.z - epsilon );
        }

        T& operator[]( const int i )
        {
            if      ( i == 0 )  { return x; }
            else if ( i == 1 )  { return y; }
            else                { return z; }
        }

        operator gs::Vec3<double>() const
        {
            return Vec3<double>( (double) x, (double) y, (double) z );
        }

        operator gs::Vec3<float>() const
        {
            return Vec3<float>( (float) x, (float) y, (float) z );
        }

    public:
        Vec3<T>()
        {
        }

        Vec3<T>( const T& x, const T& y, const T& z )
            : x( x ), y( y ), z( z )
        {
        }
    };

    typedef	Vec3<float>     Vec3f;
    typedef	Vec3<double>    Vec3d;
    typedef Vec3<int>       Vec3i;
}

#endif // GS_VEC3_H
