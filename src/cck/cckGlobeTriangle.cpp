#include "cckGlobe.h"
#include "cckMath.h"

cck::Globe::BspTree cck::Globe::Triangle::ConstructTree( const double globeRadius ) const
{
    vector<shared_ptr<Edge>> mountainEdges;
    //create Edges between triangles's center node and each side's center node
    for ( const auto& side : sides )
    {
        const std::shared_ptr<Edge> newMountainEdge( new Edge( centerNode, side->edge->centerNode, globeRadius ) );
        mountainEdges.push_back( newMountainEdge );
        side->edge->nodeA->AddToSegment( newMountainEdge ); //TODO: Only add the new edge to one segment; check with dot product against tangent
        side->edge->nodeB->AddToSegment( newMountainEdge );
    }

    BspTree tempTree;
    std::queue<bool> coord;
    tempTree.AddChildren( coord, mountainEdges.at(0) );

    bool dotNode1 = cck::DotProduct( mountainEdges.at(0)->normal, sides.at(1)->edge->centerNode->unitVec ) >= 0.0;
    coord.push( dotNode1 );
    tempTree.AddChildren( coord, mountainEdges.at(1) );

    coord.push( !dotNode1 );
    tempTree.AddChildren( coord, mountainEdges.at(2) );

    for ( int sideIndex = 1; sideIndex < 3; sideIndex++ )
    {
        for ( const auto& node : nodes )
        {
            if ( node->id == sides[sideIndex]->edge->nodeA->id || node->id == sides[sideIndex]->edge->nodeB->id )
            {
                coord.push( ( sideIndex == 1 ) ? dotNode1 : !dotNode1 );
                coord.push( cck::DotProduct( mountainEdges[sideIndex]->normal, node->unitVec ) >= 0.0 );
                tempTree.AddNode( coord, node );
            }
        }
    }

    return tempTree;
}

bool cck::Globe::Triangle::Contains( const cck::Vec3& point ) const
{
    for ( const auto& side : sides )
    {
        if ( DotProduct( point, side->normal ) < 0.0 )
        {
            return false;
        }
    }
    return true;
}

shared_ptr<cck::Globe::Node> cck::Globe::Triangle::CreateCenterNode() const
{
    for ( unsigned int i = 0; i < sides.size(); ++i )
    {
        //find weighted average point with side+1
    }

    //average average-node properties
    cck::Vec3 averagePosition;
    double averageMinHeight = 0.0;
    double averageMaxHeight = 0.0;
    double averageRadius = 0.0;
    double averagePlateau = 0.0;

    for ( const auto& side : sides )
    {
        averagePosition += side->edge->centerNode->position;
        averageMinHeight += side->edge->centerNode->minHeight;
        averageMaxHeight += side->edge->centerNode->maxHeight;
        averageRadius += side->edge->centerNode->radius;
        averagePlateau += side->edge->centerNode->plateau;
    }
    averagePosition /= 3.0;
    averageMinHeight /= 3.0;
    averageMaxHeight /= 3.0;
    averageRadius /= 3.0;
    averagePlateau /= 3.0;

    return std::make_shared<Node>( averagePosition, averageMinHeight, averageMaxHeight, averageRadius, averagePlateau );
}

vector<shared_ptr<cck::Globe::Node>> cck::Globe::Triangle::CreateNodeVector( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const shared_ptr<Node>& nodeC ) const
{
    vector<shared_ptr<Node>> tempVector;
    tempVector.push_back( nodeA );
    tempVector.push_back( nodeB );
    tempVector.push_back( nodeC );
    return tempVector;
}

double cck::Globe::Triangle::GetInfluence( const cck::Vec3& samplePoint ) const
{
    return Contains( samplePoint ) ? 1.0 : 0.0;
}

bool cck::Globe::Triangle::SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const
{
    if ( Contains( samplePoint ) )
    {
        tree.SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId );
        return true;
    }

    return false;
}

cck::Globe::Triangle::Triangle( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const shared_ptr<Node>& nodeC, const vector<shared_ptr<Side>>& sides, const double globeRadius )
    :   nodes( CreateNodeVector( nodeA, nodeB, nodeC ) ),
        sides( sides ),
        centerNode( CreateCenterNode() ),
        tree( ConstructTree( globeRadius ) )
{
}
