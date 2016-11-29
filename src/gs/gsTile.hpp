#pragma once

#include "../gl3w/gl3w.h"
#include <vector>
#include <memory>

#include "gsLink.hpp"
#include "gsVec3.hpp"
#include "gsVertex.hpp"
#include "gsTexture.hpp"
#include "gsWaterTextureGenerator.hpp"

using std::shared_ptr;
using std::vector;

namespace gs
{
    class LandTile;
    class Vertex;
    class WaterTile;

    class Tile
    {
    public:
        enum class Type
        {
            UNASSIGNED,
            LAND,
            WATER
        };

    public:
        const int                               id;

    protected:
        static int                              idCounter;
        Type                                    surface;
        const gs::Vec3d                         normal;
        const double                            height;

        const vector<shared_ptr<gs::Vertex>>    vertices;
		const gs::Vec3d							center;
        GLuint                                  bufferOffset;
        GLuint                                  indexBufferOffset;
        gs::Vec3d                               color;
        bool                                    fog;

		static gs::WaterTextureGenerator		waterTextureGenerator;
		shared_ptr<gs::Texture>					texture; //TODO: should be on stack, or at least unique_ptr
		vector<gs::Vec2d>						texCoords;

        vector<gs::Link<gs::Tile>>              allLinks;
        vector<gs::Link<gs::LandTile>>          landLinks;
        vector<gs::Link<gs::WaterTile>>         waterLinks;

        //owner
        //controller
    protected:
        void									UpdateColorBuffer(const GLuint colorVbo);
        void									UpdateFogBuffer(const GLuint fogVbo);
        void									UpdatePositionBuffer(const GLuint positionVbo);

    public:
        void									AddVerticesToIndexVector(vector<GLuint>& indexVector);

        bool									AddLink(const gs::Link<gs::Tile>& link);
        bool									AddLink(const gs::Link<gs::LandTile>& link);
        bool									AddLink(const gs::Link<gs::WaterTile>& link);

        GLuint									GetBufferEnd() const;
        GLuint									GetBufferOffset() const;
        double									GetHeight() const;
        GLuint									GetIndexBufferEnd() const;
        GLuint									GetIndexBufferOffset() const;
        size_t									GetNumOfVertices() const;
        Type									GetSurface() const;

		vector<gs::Link<gs::LandTile>>			GetLandLinks() const;

        GLuint									SetBufferOffset(const GLuint offset);

		void									AddToTileGroupTexture(shared_ptr<gs::Texture> tileGroupTexture, const gs::Vec2i& tileGroupTextureOffset, const int tileGroupTextureSize);
		void									DeleteLocalTextureData();
		virtual void							GenerateTexture() = 0;
		shared_ptr<gs::Texture>					GetTexture() const;

		void									UpdateTexCoordBuffer(const GLuint texCoordVbo);
		void									UpdateAllBuffers(const GLuint positionVbo, const GLuint colorVbo, const GLuint fogVbo, const GLuint texCoordVbo);

	private:
		gs::Vec3d								CalculateCenter() const;

    protected:
        Tile(const Type surface, const vector<shared_ptr<gs::Vertex>>& vertices, const gs::Vec3d& centroid, const double height);

    public:
        //Tile(const int bufferOffset, const vector<shared_ptr<gs::Vertex>>& vertices);
        virtual ~Tile();
    };

    typedef shared_ptr<gs::Tile> TilePtr;
}