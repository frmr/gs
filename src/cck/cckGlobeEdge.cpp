#include "cckGlobe.h"
#include "cckMath.h"

#include <limits>

cck::Vec3 cck::Globe::Edge::ClosestPoint( const cck::Vec3& samplePoint ) const
{
    return cck::Vec3( samplePoint - normal * cck::DotProduct( normal, samplePoint ) );
}

cck::Globe::BspTree cck::Globe::Edge::ConstructTree( const double mountainMinHeight, const double mountainMaxHeight, const double mountainRadius, const double mountainPlateau, const double globeRadius )
{
    //construct nodes either side of center node
    const shared_ptr<Node> positiveNode( new Node( centerNode->position + normal * globeRadius, mountainMinHeight, mountainMaxHeight, mountainRadius, mountainPlateau ) );
    const shared_ptr<Node> negativeNode( new Node( centerNode->position - normal * globeRadius, mountainMinHeight, mountainMaxHeight, mountainRadius, mountainPlateau ) );

    positiveMountain = std::make_shared<Edge>( centerNode, positiveNode, globeRadius );
    negativeMountain = std::make_shared<Edge>( centerNode, negativeNode, globeRadius );

    //construct BspTree
    BspTree tempTree;
    std::queue<bool> coord;
    tempTree.AddChildren( coord, positiveMountain );

    const bool nodeDotProduct = cck::DotProduct( positiveMountain->normal, nodeA->unitVec ) >= 0.0;

    coord.push( nodeDotProduct );
    tempTree.AddNode( coord, nodeA );
    nodeA->AddToSegment( positiveMountain );
    nodeA->AddToSegment( negativeMountain );
    nodeA->AddToSegment( centerNode );

    coord.push( !nodeDotProduct );
    tempTree.AddNode( coord, nodeB );
    nodeB->AddToSegment( positiveMountain );
    nodeB->AddToSegment( negativeMountain );
    nodeB->AddToSegment( centerNode );

    return tempTree;
}

bool cck::Globe::Edge::Contains( const cck::Vec3& point ) const
{
    const double dot12 = cck::DotProduct( nodeA->unitVec, nodeB->unitVec );
    const double dot1p = cck::DotProduct( nodeA->unitVec, point );
    const double dot2p = cck::DotProduct( nodeB->unitVec, point );

    const double invDenom = 1.0 / ( dot12 * dot12 );
    const double u = ( dot1p - dot12 * dot2p ) * invDenom;
    const double v = ( dot2p - dot12 * dot1p ) * invDenom;

    return ( u < 0.0 || v < 0.0 ) ? false : true;
}

bool cck::Globe::Edge::PointOnFreeSide( const cck::Vec3& samplePoint ) const
{
    for ( const auto& side : sides )
    {
        if ( !side->FormsTriangle() )
        {
            if ( cck::DotProduct( samplePoint, side->normal ) >= 0.0 )
            {
                return true;
            }
        }
    }

    return false;
}

void cck::Globe::Edge::AddSides()
{
    sides.push_back( std::make_shared<Side>( nodeA, nodeB, shared_from_this() ) );
    sides.push_back( std::make_shared<Side>( nodeB, nodeA, shared_from_this() ) );
}

double cck::Globe::Edge::GetDistance( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius ) const
{
    const cck::Vec3 closest = ClosestPoint( samplePoint );

    if ( Contains( closest ) )
    {
        const cck::GeoCoord closestCoord = closest.ToGeographic();
        const double closestDistToNodeA = cck::Distance( nodeA->coord, closestCoord, globeRadius );
        const double edgeRadius = nodeA->radius + ( ( nodeB->radius - nodeA->radius ) * ( closestDistToNodeA / length ) );
        const double distance = cck::Distance( sampleCoord, closestCoord, globeRadius );

        if ( distance <= edgeRadius )
        {
            return distance;
        }
    }
    return std::numeric_limits<double>::max();
}

double cck::Globe::Edge::GetInfluence( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius ) const
{
    if ( PointOnFreeSide( samplePoint ) )
    {
        const cck::Vec3 closest = ClosestPoint( samplePoint );

        if ( Contains( closest ) )
        {
            const cck::GeoCoord closestCoord = closest.ToGeographic();
            const double maxDist = nodeA->radius + ( nodeB->radius - nodeA->radius ) * cck::Distance( nodeA->coord, closestCoord, globeRadius ) / length;
            const double distance = cck::Distance( sampleCoord, closestCoord, globeRadius );
            if ( distance <= maxDist )
            {
                const double fraction = distance / maxDist;
                //return 1.0 - ( fraction * fraction );
                return 1.0 - fraction;
            }
        }
    }
    return 0.0;
}

double cck::Globe::Edge::GetMountainHeight( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, const double segmentHeight ) const
{
    const cck::Vec3 closest = ClosestPoint( samplePoint );

    if ( Contains( closest ) )
    {
        const cck::GeoCoord closestCoord = closest.ToGeographic();
        const double closestDistToNodeA = cck::Distance( nodeA->coord, closestCoord, globeRadius );
        const double edgeRadius = nodeA->radius + ( ( nodeB->radius - nodeA->radius ) * ( closestDistToNodeA / length ) );
        const double distance = cck::Distance( sampleCoord, closestCoord, globeRadius );

        if ( distance <= edgeRadius )
        {
            const double edgeMinHeight = nodeA->minHeight + ( nodeB->minHeight - nodeA->minHeight ) * ( closestDistToNodeA / length );
            const double edgeMaxHeight = nodeA->maxHeight + ( nodeB->maxHeight - nodeA->maxHeight ) * ( closestDistToNodeA / length );
            const double edgeHeight = edgeMinHeight + noiseValue * ( edgeMaxHeight - edgeMinHeight );
            const double edgePlateau = nodeA->plateau + ( nodeB->plateau - nodeA->plateau ) * ( closestDistToNodeA / length );

            return ( distance <= edgePlateau ) ? edgeHeight : cck::Globe::CalculateMountainHeight( segmentHeight, edgeHeight, edgeRadius, edgePlateau, distance );
        }
    }
    return std::numeric_limits<double>::lowest();
}

vector<shared_ptr<cck::Globe::Side>> cck::Globe::Edge::GetSides() const
{
    return sides;
}

bool cck::Globe::Edge::IsActive() const
{
    return active;
}

void cck::Globe::Edge::SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const
{
    tree.SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId );
}

void cck::Globe::Edge::SetInactive()
{
    active = false;
}

cck::Globe::Edge::Edge( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const double mountainMinHeight, const double mountainMaxHeight, const double mountainRadius, const double mountainPlateau, const double globeRadius )
    :   active( true ),
        positiveMountain( nullptr ),
        negativeMountain( nullptr ),
        nodeA( nodeA ),
        nodeB( nodeB ),
        length( cck::Distance( nodeA->coord, nodeB->coord, globeRadius ) ),
        //centerNode( std::make_shared<Node>( nodeA->coord.ToCartesian( globeRadius ) + ( ( nodeB->coord.ToCartesian( globeRadius ) - nodeA->coord.ToCartesian( globeRadius ) ) * nodeA->radius / ( nodeA->radius + nodeB->radius ) ),
                                            //mountainMinHeight, mountainMaxHeight, mountainRadius, mountainPlateau ) ),
        centerNode( std::make_shared<Node>( ( nodeA->coord.ToCartesian( globeRadius ) * nodeB->radius + nodeB->coord.ToCartesian( globeRadius ) * nodeA->radius ) / ( nodeA->radius + nodeB->radius ), mountainMinHeight, mountainMaxHeight, mountainRadius, mountainPlateau ) ),
        normal( cck::CrossProduct( nodeA->unitVec, nodeB->unitVec ).Unit() ),
        tree( ConstructTree( mountainMinHeight, mountainMaxHeight, mountainRadius, mountainPlateau, globeRadius ) )
{
}

cck::Globe::Edge::Edge( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const double globeRadius )
    :   active( true ),
        positiveMountain( nullptr ),
        negativeMountain( nullptr ),
        nodeA( nodeA ),
        nodeB( nodeB ),
        length( cck::Distance( nodeA->coord, nodeB->coord, globeRadius ) ),
        centerNode( nullptr ), //mountain Edge has no center Node
        normal( cck::CrossProduct( nodeA->unitVec, nodeB->unitVec ).Unit() ),
        tree()
{
}
