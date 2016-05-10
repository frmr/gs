#include "gsEdge.h"
#include "gsTile.h"
#include "gsVertex.h"

#include <iostream>

int gs::Edge::idCounter = 0;

bool gs::Edge::AddTile(const shared_ptr<gs::Tile>& newTile)
{
    if (tiles.size() == 2)
    {
        return false;
    }
    tiles.push_back(newTile);
    return true;
}

bool gs::Edge::BordersTile(const shared_ptr<gs::Tile>& refTile) const
{
    for (const auto& tile : tiles)
    {
        if (tile == refTile)
        {
            return true;
        }
    }
    return false;
}

vector<shared_ptr<gs::Tile>> gs::Edge::GetTiles() const
{
    return tiles;
}

bool gs::Edge::HasVertex(const shared_ptr<gs::Vertex>& refVertex) const
{
    return (v0->position == refVertex->position || v1->position == refVertex->position);
}

bool gs::Edge::IsRiver() const
{
    return river;
}

void gs::Edge::SetRiver()
{
    river = true;
}

void gs::Edge::Widen() //TODO: Stop the method shortening some edges
{
    constexpr float minimumLength = 0.005f;
    const float length = (float) vec.Length();
    if (length < minimumLength)
    {
        vec.Unit();
        const gs::Vec3d correction = vec * ((minimumLength - length) / 2.0f);
        v1->position = v1->position + correction;
        v0->position = v0->position - correction;
        vec = v1->position - v0->position;
        //std::cerr << length << " Widened edge to by " << correction.Length() * 2.0f << " or " << minimumLength - length << " to " << vec.Length() << std::endl;
    }
}

gs::Edge::Edge(const shared_ptr<gs::Vertex>& v0, const shared_ptr<gs::Vertex>& v1)
    :   river(false),
        vec(v1->position - v0->position),
        id(idCounter++),
        v0(v0),
        v1(v1)
{
}
