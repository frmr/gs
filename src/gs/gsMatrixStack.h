#ifndef GS_MATRIX_STACK_H
#define GS_MATRIX_STACK_H

#include <stack>

#include <GL/gl3w.h>

namespace gs
{
    template<typename T>
    class MatrixStack
    {
    private:
        std::stack<T> matrices;

    public:
        T top;

    public:
        void SendUniform( const GLint location ) const
        {
            glUniformMatrix4fv( location, 1, false, top.get() );
        }

        void Push()
        {
            matrices.push( top );
        }

        void Pop()
        {
            top = matrices.top();
            matrices.pop();
        }

    };
}

#endif // GS_MATRIX_STACK_H
