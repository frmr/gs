#include "gsTileBuffer.h"

void gs::TileBuffer::Bind() const
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

GLuint gs::TileBuffer::CreateVbo(const int elements, const int components, gs::Shader& shader, const string& name)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint location = shader.GetAttribLocation(name);

    glBufferData(GL_ARRAY_BUFFER, elements * components * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);

    return vbo;
}

void gs::TileBuffer::InitIndexBuffer(const vector<GLuint>& indexVector) const
{
    GLuint* indexArray = new GLuint[indexVector.size()];
    for (unsigned int i = 0; i < indexVector.size(); ++i)
    {
        indexArray[i] = indexVector[i];
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVector.size() * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

    delete[] indexArray;
}

bool gs::TileBuffer::Update(const GLuint tileIndex, const gs::Vec3f& newColor, const bool newFog)
{
    return true;
}

gs::TileBuffer::TileBuffer(const int size, gs::Shader& shader, const vector<GLuint>& indexVector)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    positionVbo = CreateVbo(size, 3, shader, "positionVert");
    colorVbo = CreateVbo(size, 3, shader, "colorVert");
    fogVbo = CreateVbo(size, 1, shader, "fogVert");

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    InitIndexBuffer(indexVector);
}
