#ifndef GS_MATRIX_STACK_H
#define GS_MATRIX_STACK_H

#include <stack>

#include <GL/gl3w.h>

#include "../matrix/Matrices.h"

namespace gs
{
    class MatrixStack
    {
    private:
        std::stack<Matrix4> matrices;

    public:
        Matrix4 top;

    public:
        void    SendUniform( const GLint location ) const;
        void    Push();
        void    Pop();

    };
}

#endif // GS_MATRIX_STACK_H
