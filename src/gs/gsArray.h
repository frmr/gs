#ifndef GS_ARRAY_H
#define GS_ARRAY_H

#include <vector>

using std::vector;

namespace gs
{
    template<typename T>
    class Array
    {
    private:
        const vector<size_t>    dims;
        T*                      data;

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
            if ( data != NULL )
            {
                delete[] data;
                data = NULL;
            }
        }

    public:
        Array( const size_t xDim )
            :   dims( { xDim } ),
                data( new T[xDim] )
        {
        }

        Array( const size_t xDim, const size_t yDim )
            :   dims( { xDim, yDim } ),
                data( new T[xDim * yDim] )
        {
        }

        Array( const size_t xDim, const size_t yDim, const size_t zDim )
            :   dims( { xDim, yDim, zDim } ),
                data( new T[xDim * yDim * zDim] )
        {
        }

        ~Array()
        {
            Delete();
        }
    };
}

#endif // GS_ARRAY3_H