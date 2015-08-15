#include "cckGlobe.h"

#include <limits>

void cck::Globe::Segment::AddEdge( const shared_ptr<Edge>& newEdge )
{
    mountainEdges.push_back( newEdge );
}

void cck::Globe::Segment::AddNode( const shared_ptr<Node>& newNode )
{
    mountainNodes.push_back( newNode );
}

void cck::Globe::Segment::SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const
{
    sampleId = baseNode->id;
    const double segmentHeight = baseNode->minHeight + noiseValue * ( baseNode->maxHeight - baseNode->minHeight );

    //double highestMountain = std::numeric_limits<double>::lowest();
    bool foundMountain = false;
    double closestDistance = std::numeric_limits<double>::max();
    shared_ptr<Edge> closestEdge = nullptr;

    for ( const auto& edge : mountainEdges )
    {
        if ( edge->IsActive() )
        {
            const double distance = edge->GetDistance( sampleCoord, samplePoint, globeRadius );
            if ( distance < closestDistance )
            {
                closestDistance = distance;
                closestEdge = edge;
                foundMountain = true;
            }
        }
    }

    if ( foundMountain )
    {
        sampleHeight = closestEdge->GetMountainHeight( sampleCoord, samplePoint, globeRadius, noiseValue, segmentHeight );
        return;
    }

    shared_ptr<Node> closestNode = nullptr;
    for ( const auto& node : mountainNodes )
    {
        const double distance = node->GetDistance( sampleCoord, globeRadius );
        if ( distance < closestDistance )
        {
            closestDistance = distance;
            closestNode = node;
            foundMountain = true;
        }
    }
    if ( foundMountain )
    {
        sampleHeight = closestNode->GetMountainHeight( sampleCoord, globeRadius, noiseValue, segmentHeight );
        return;
    }

    sampleHeight = segmentHeight;
}

cck::Globe::Segment::Segment( const shared_ptr<Node>& baseNode, const vector<shared_ptr<Node>>& mountainNodes, const vector<shared_ptr<Edge>>& mountainEdges )
    :   mountainNodes( mountainNodes ),
        mountainEdges( mountainEdges ),
        baseNode( baseNode )
{
}

cck::Globe::Segment::Segment( const shared_ptr<Node>& baseNode )
    :   baseNode( baseNode )
{
}
