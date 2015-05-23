#include "gsMatrixStack.h"

void gs::MatrixStack::SendUniform( const GLint location ) const
{
    glUniformMatrix4fv( location, 1, false, top.get() );
}

void gs::MatrixStack::Push()
{
    matrices.push( top );
}

void gs::MatrixStack::Pop()
{
    top = matrices.top();
    matrices.pop();
}
