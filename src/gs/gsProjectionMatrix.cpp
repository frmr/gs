#include "gsProjectionMatrix.h"

bool gs::ProjectionMatrix::SetOrthographic( const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal )
{
    (*this)[0] = 2 / ( right - left );
    (*this)[5]  = 2 / ( top - bottom );
    (*this)[10] = -2 / ( farVal - nearVal );
    (*this)[12] = -( right + left ) / ( right - left );
    (*this)[13] = -( top + bottom ) / ( top - bottom );
    (*this)[14] = -( farVal + nearVal ) / ( farVal - nearVal );
}

gs::ProjectionMatrix::ProjectionMatrix()
    :   Matrix4()
{
}
