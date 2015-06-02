#include "cckGlobe.h"
#include "cckMath.h"

bool cck::Globe::BspTree::BspNode::AddChildren( std::queue<bool>& coord, const shared_ptr<Edge>& newEdge )
{
    if ( node == nullptr )
    {
        if ( coord.empty() )
        {
            if ( edge == nullptr )
            {
                edge = newEdge;
                posChild = std::unique_ptr<BspNode>( new BspNode() );
                negChild = std::unique_ptr<BspNode>( new BspNode() );
                return true;
            }
        }
        else
        {
            if ( posChild == nullptr || negChild == nullptr )
            {
                return false;
            }

            if ( coord.front() == true )
            {
                if ( posChild != nullptr )
                {
                    coord.pop();
                    return posChild->AddChildren( coord, newEdge );
                }
            }
            else
            {
                if ( negChild != nullptr )
                {
                    coord.pop();
                    return negChild->AddChildren( coord, newEdge );
                }
            }
        }
    }
    return false;
}

bool cck::Globe::BspTree::BspNode::AddNode( std::queue<bool>& coord, const shared_ptr<Node>& newNode )
{
    if ( coord.empty() )
    {
        if ( node == nullptr )
        {
            node = newNode;
            return true;
        }
    }
    else
    {
        if ( coord.front() == true )
        {
            if ( posChild != nullptr )
            {
                coord.pop();
                return posChild->AddNode( coord, newNode );
            }
        }
        else
        {
            if ( negChild != nullptr )
            {
                coord.pop();
                return negChild->AddNode( coord, newNode );
            }
        }
    }
    return false;
}

bool cck::Globe::BspTree::BspNode::IsComplete() const
{
    if ( node != nullptr )
    {
        return true;
    }
    else
    {
        if ( posChild == nullptr || negChild == nullptr )
        {
            return false;
        }
        else if ( posChild->IsComplete() && negChild->IsComplete() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void cck::Globe::BspTree::BspNode::SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const
{
    if ( node == nullptr )
    {
        if ( cck::DotProduct( edge->normal, samplePoint ) >= 0.0 )
        {
            return posChild->SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId );
        }
        else
        {
            return negChild->SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId );
        }
    }
    else
    {
        node->GetSegment()->SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId );
    }
}

cck::Globe::BspTree::BspNode::BspNode()
    :   edge( nullptr ),
        node( nullptr ),
        posChild( nullptr ),
        negChild( nullptr )
{
}

bool cck::Globe::BspTree::AddChildren( std::queue<bool>& coord, const shared_ptr<Edge>& newEdge )
{
    return root.AddChildren( coord, newEdge );
}

bool cck::Globe::BspTree::AddNode( std::queue<bool>& coord, const shared_ptr<Node>& newNode )
{
    return root.AddNode( coord, newNode );
}

bool cck::Globe::BspTree::IsComplete() const
{
    return root.IsComplete();
}

void cck::Globe::BspTree::SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const
{
    root.SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId );
}

cck::Globe::BspTree::BspTree()
{
}
