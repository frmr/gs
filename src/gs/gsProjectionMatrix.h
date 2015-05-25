#ifndef GS_PROJECTION_MATRIX_H
#define GS_PROJECTION_MATRIX_H

#include "../matrix/Matrices.h"

namespace gs
{
    class ProjectionMatrix : public Matrix4
    {
    public:
        bool SetOrthographic( const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal );
    public:
        ProjectionMatrix();
    };
}

#endif // GS_PROJECTION_MATRIX_H
