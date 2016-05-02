#ifndef GS_FULLSCREEN_QUAD_H
#define GS_FULLSCREEN_QUAD_H

#include "../gl3w/gl3w.h"

namespace gs
{
    class FullscreenQuad
    {
    private:
        GLuint vao;
        GLuint vbo;

    public:
        void Render()
        {

        }

    public:
        FullscreenQuad(const int width, const int height)
        {
            const float widthf = (float) width;
            const float heightf = (float) height;
            //define quad (position, tex coords)
            constexpr numVertices = 30;
            float data[dataLength] = {  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, heightf, 0.0f, 0.0f, 1.0f, widthf, 0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, heightf, 0.0f, 0.0f, 1.0f, widthf, heightf, 0.0f, 1.0f, 1.0f, widthf, 0.0f, 0.0f, 1.0f, 0.0f };

            //generate vao
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            //generate vbo
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dataLength, &data[0], GL_STATIC_DRAW);
            //define vertices and texture coordinates
            //fill vbo
        }
    };
}

#endif
