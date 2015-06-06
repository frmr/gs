#ifndef GS_EDGE_H
#define GS_EDGE_H

#include "gsTile.h"
#include "gsVec3.h"

#include <memory>

using std::shared_ptr;

namespace gs
{
    class Tile;

    class Edge
    {
    private:
        vector<shared_ptr<gs::Tile>> tiles;

    public:
        const gs::Vec3f v0;
        const gs::Vec3f v1;

    public:
        bool                            AddTile( const shared_ptr<gs::Tile>& newTile );
        bool                            BordersTile( const shared_ptr<gs::Tile>& refTile ) const;
        vector<shared_ptr<gs::Tile>>    GetTiles() const;
        bool                            HasVertices( const gs::Vec3f& v0, const gs::Vec3f& v1 ) const;

    public:
        Edge( const gs::Vec3f& v0, const gs::Vec3f& v1 );
    };
}

#endif // GS_EDGE_H
