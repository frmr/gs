#include "gsVertex.h"
#include "gsEdge.h"
#include "gsTile.h"

#include <algorithm>
#include <iostream>
#include <limits>

using std::cerr;
using std::endl;

int gs::Vertex::idCounter = 0;

void gs::Vertex::AddLink(const gs::Link<gs::Vertex>& link)
{
    links.push_back(link);
}

void gs::Vertex::AddTile(const shared_ptr<gs::Tile> tile)
{
    tiles.push_back(tile);
}

void gs::Vertex::CalculateHeight()
{
    double total = 0;
    for (const auto& tile : tiles)
    {
        total += tile->GetHeight();
    }
    height = total / (double) tiles.size();
}

vector<shared_ptr<gs::Edge>> gs::Vertex::GetEdges() const
{
    return edges;
}

shared_ptr<gs::Edge> gs::Vertex::GetEdgeWith(const shared_ptr<gs::Vertex> refVertex) const
{
    for (auto link : links)
    {
        if (link.edge->HasVertex(refVertex))
        {
            return link.edge;
        }
    }
    return nullptr;
}

double gs::Vertex::GetHeight() const
{
    return height;
}

//gs::Vec3f gs::Vertex::GetPosition() const
//{
//    return position;
//}

bool gs::Vertex::IsRiver() const
{
    return (riverId != -1);
}
//
//void gs::Vertex::SetPosition(const gs::Vec3f& newPosition)
//{
//    position = newPosition;
//}

bool gs::Vertex::SetRiver(const int newRiverId)
{
    if (riverId == newRiverId)
    {
        //river converges with itself, which is impossible
        return false;
    }
    else if (riverId != -1)
    {
        //two rivers converge
        return true;
    }

    //else, the vertex is not already a river

    //stop if vertex touches the sea
    for (const auto& tile : tiles)
    {
        if (tile->GetSurface() == gs::Tile::Type::WATER)
        {
            return true;
        }
    }

    riverId = newRiverId;

    vector<int> visitedIds;

    bool childSucceeded = false;

    while (!childSucceeded)
    {
        gs::EdgePtr lowestEdge = nullptr;
        gs::VertexPtr lowestVertex = nullptr;
        double lowestHeight = std::numeric_limits<double>::max();

        for (auto& link : links)
        {
            bool targetVisited = (std::find(visitedIds.begin(), visitedIds.end(), link.target->id) != visitedIds.end());
            if (!targetVisited && link.target->GetHeight() < lowestHeight)
            {
                lowestEdge = link.edge;
                lowestVertex = link.target;
                lowestHeight = link.target->GetHeight();
            }
        }

        if (lowestEdge == nullptr)
        {
            riverId = -1;
            return false;
        }
        else
        {
            childSucceeded = lowestVertex->SetRiver(newRiverId);
            if (childSucceeded)
            {
                lowestEdge->SetRiver();
            }
            else
            {
                visitedIds.push_back(lowestVertex->id);
            }
        }

    }
    return true;
}

gs::Vertex::Vertex(const gs::Vec3f& position)
    :   id(idCounter++),
        position(position),
        height(0.0),
        riverId(-1)
{
}
