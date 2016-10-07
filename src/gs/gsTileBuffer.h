#pragma once

#include "../gl3w/gl3w.h"

#include "gsShader.h"
#include "gsTile.h"
#include "gsVec3.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace gs
{
	template<typename T>
    class TileBuffer
    {
    protected:
        GLuint  vao;
        GLuint  positionVbo;
        GLuint  colorVbo;
        GLuint  fogVbo;
        GLuint  indexBuffer;
		GLuint  texCoordVbo;

    private:
        void InitIndexBuffer(const vector<GLuint>& indexVector) const
		{
			GLuint* indexArray = new GLuint[indexVector.size()];
			for (unsigned int i = 0; i < indexVector.size(); ++i)
			{
				indexArray[i] = indexVector[i];
			}

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVector.size() * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

			delete[] indexArray;
		}

		vector<GLuint> BuildIndexVector(vector<std::shared_ptr<T>>& tiles) const
		{
			vector<GLuint> indexVector;
			for (auto& tile : tiles)
			{
				tile->AddVerticesToIndexVector(indexVector);
			}
			return indexVector;
		}

		int CountVertices(const vector<std::shared_ptr<T>>& tiles) const
		{
			int numOfVertices = 0;
			for (const auto& tile : tiles)
			{
				numOfVertices += int(tile->GetNumOfVertices());
			}
			return numOfVertices;
		}

    protected:
        GLuint CreateVbo(const int elements, const int components, gs::Shader& shader, const string& name)
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

    public:
        void Bind() const
		{
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		}

		bool Update(const GLuint tileIndex, const gs::Vec3d& newColor, const bool newFog)
		{
			return true;
		}

		void UpdateTexCoordBuffer(const std::shared_ptr<T>& tile) const
		{
			tile->UpdateTexCoordBuffer(texCoordVbo);
		}

    public:
        TileBuffer(std::vector<std::shared_ptr<T>>& tiles, gs::Shader& shader)
		{
			const int size = CountVertices(tiles);
			const vector<GLuint>& indexVector = BuildIndexVector(tiles);
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			positionVbo = CreateVbo(size, 3, shader, "positionVert");
			colorVbo = CreateVbo(size, 3, shader, "colorVert");
			fogVbo = CreateVbo(size, 1, shader, "fogVert");
			texCoordVbo = CreateVbo(size, 2, shader, "texCoordVert");

			glGenBuffers(1, &indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			InitIndexBuffer(indexVector);

			for (auto& tile : tiles)
			{
				tile->UpdateAllBuffers(positionVbo, colorVbo, fogVbo, texCoordVbo);
			}
		}

    };
}