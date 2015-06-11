#ifndef GS_ARRAY3_H
#define GS_ARRAY3_H

#include <vector>

using std::vector;

namespace gs
{
    template<typename T>
    class Array
    {
    private:
        const vector<size_t>    dims;
        vector<T>               data;

    public:
        T&  At( const size_t i )
        {
            return data[ i ];
        }

        T& At( const size_t i, const size_t j )
        {
            return data[ i + j * dims[0] ];
        }

        T& At( const size_t i, const size_t j, const size_t k )
        {
            return data[ i + j * dims[0] + k * dims[0] * dims[1] ];
        }

        void Delete()
        {
            data.clear();
        }

    public:
        Array( const size_t xDim )
            :   dims( { xDim } )
        {
            data.resize( xDim );
        }

        Array( const size_t xDim, const size_t yDim )
            :   dims( { xDim, yDim } )
        {
            data.resize( xDim * yDim );
        }

        Array( const size_t xDim, const size_t yDim, const size_t zDim )
            :   dims( { xDim, yDim, zDim } )
        {
            data.resize( xDim * yDim * zDim );
        }
    };
}

#endif // GS_ARRAY3_H
