#pragma once

#include <stack>

#include "../gl3w/gl3w.h"
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
        void SendUniform(const GLint location) const
        {
            glUniformMatrix4fv(location, 1, false, top.get());
        }

        void Push()
        {
            matrices.push(top);
        }

        void Pop()
        {
            top = matrices.top();
            matrices.pop();
        }

    };
}
