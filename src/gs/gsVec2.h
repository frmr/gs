#pragma once

#include <cmath>
#include <limits>

namespace gs
{
    template<typename T>
    class Vec2
    {
    public:
        T   x, y;
        static constexpr T epsilon = (T) 0.0000001;
        static constexpr uint8_t dimensions = 2;

    public:

        double Length() const
        {
            return sqrt(x * x + y * y);
        }

        Vec2<T>& Reverse()
        {
            x = -x;
            y = -y;
            return *this;
        }

        Vec2<T>& Unit()
        {
            T length = Length();
            x /= length;
            y /= length;
            return *this;
        }


        template<typename U>
        Vec2<T> operator+(const Vec2<U>& rhs) const
        {
            return gs::Vec2<T>(x + (T) rhs.x, y + (T) rhs.y);
        }

        template<typename U>
        Vec2<T> operator-(const Vec2<U>& rhs) const
        {
            return gs::Vec2<T>(x - (T) rhs.x, y - (T) rhs.y);
        }

        template<typename U>
        Vec2<T> operator*(const U& rhs) const
        {
            return gs::Vec2<T>(x * (T) rhs, y * (T) rhs);
        }

        template<typename U>
        Vec2<T> operator*(const Vec2<U>& rhs) const
        {
            return gs::Vec2<T>(x * (T) rhs.x, y * (T) rhs.y);
        }

        template<typename U>
        Vec2<T> operator/(const U& rhs) const
        {
            return gs::Vec2<T>(x / (T) rhs, y / (T) rhs);
        }

        template<typename U>
        Vec2<T>& operator+=(const Vec2<U>& rhs)
        {
            x += (T) rhs.x;
            y += (T) rhs.y;
            return *this;
        }

        template<typename U>
        Vec2<T>& operator-=(const Vec2<U>& rhs)
        {
            x -= (T) rhs.x;
            y -= (T) rhs.y;
            return *this;
        }

        template<typename U>
        Vec2<T>& operator*=(const U& rhs)
        {
            x *= (T) rhs;
            y *= (T) rhs;
            return *this;
        }

        template<typename U>
        Vec2<T>& operator*=(const Vec2<U>& rhs)
        {
            x *= (T) rhs.x;
            y *= (T) rhs.y;
            return *this;
        }

        template<typename U>
        Vec2<T>& operator/=(const U& rhs)
        {
            x /= (T) rhs;
            y /= (T) rhs;
            return *this;
        }

        template<typename U>
        bool operator==(const U& rhs) const
        {
            return (x <= (T) rhs.x + epsilon &&
                     x >= (T) rhs.x - epsilon &&
                     y <= (T) rhs.y + epsilon &&
                     y >= (T) rhs.y - epsilon);
        }

        T& operator[](const int i)
        {
            return (i == 0) ? x : y;
        }

        explicit operator gs::Vec2<double>() const
        {
            return Vec2<double>((double) x, (double) y);
        }

        explicit operator gs::Vec2<float>() const
        {
            return Vec2<float>((float) x, (float) y);
        }

    public:
        Vec2<T>()
        {
        }

        Vec2<T>(const T& x, const T& y)
            : x(x), y(y)
        {
        }

        static Vec2<T> Max()
        {
            return Vec2<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
        }

        static Vec2<T> Min()
        {
            return Vec2<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest());
        }
    };

    typedef	Vec2<float>     Vec2f;
    typedef	Vec2<double>    Vec2d;
    typedef Vec2<int>       Vec2i;
}