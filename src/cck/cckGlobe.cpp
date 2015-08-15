#include "cckGlobe.h"

#include "cckMath.h"

#include <cmath>
#include <limits>
#include <map>

double cck::Globe::CalculateMountainHeight( const double segmentHeight, const double mountainHeight, const double radius, const double plateau, const double distance )
{
    //return segmentHeight + ( ( mountainHeight - segmentHeight ) * ( sin( ( ( cck::pi * ( 1.0 - ( ( distance - plateau ) / ( radius - plateau ) ) ) ) - cck::halfPi ) ) + 1.0 ) / 2.0 );
    return segmentHeight + ( 1.0 - ( ( distance - plateau ) / ( radius - plateau ) ) ) * ( mountainHeight - segmentHeight );
}

cck::NodeError cck::Globe::AddNode( const int id, const double latitude, const double longitude, const double minHeight, const double maxHeight, const double nodeRadius )
{
    return AddNode( id, cck::GeoCoord( latitude * cck::pi / 180.0, longitude * cck::pi / 180.0 ), minHeight, maxHeight, nodeRadius );
}

cck::NodeError cck::Globe::AddNode( const int id, const cck::GeoCoord& coord, const double minHeight, const double maxHeight, const double nodeRadius )
{
    if ( id < 0 )
    {
        return cck::NodeError::NEGATIVE_ID;
    }

    for ( const auto& nodeIt : nodes )
    {
        if ( nodeIt->id == id ) return cck::NodeError::ID_ALREADY_IN_USE;
    }

    if ( coord.latRadians < -cck::halfPi || coord.latRadians > cck::halfPi )
    {
        return cck::NodeError::LATITUDE_OUT_OF_RANGE;
    }

    if ( coord.lonRadians < -cck::pi || coord.lonRadians > cck::pi )
    {
        return cck::NodeError::LONGITUDE_OUT_OF_RANGE;
    }

    if ( nodeRadius < 0.0 )
    {
        return cck::NodeError::NEGATIVE_RADIUS;
    }
    else if ( nodeRadius > cck::pi * globeRadius )
    {
        return cck::NodeError::DIAMETER_EXCEEDS_SPHERE_CIRCUMFERENCE;
    }

    nodes.push_back( std::make_shared<Node>( id, coord, minHeight, maxHeight, nodeRadius, globeRadius ) );

    return cck::NodeError::SUCCESS;
}

cck::LinkError cck::Globe::LinkNodes( const int nodeIdA, const int nodeIdB, const double mountainMinHeight, const double mountainMaxHeight, const double mountainRadius, const double mountainPlateau )
{
    if ( nodeIdA < 0 || nodeIdB < 0 )
    {
        return cck::LinkError::NEGATIVE_ID;
    }

    if ( nodeIdA == nodeIdB )
    {
        return cck::LinkError::DUPLICATE_ID;
    }

    shared_ptr<Node> nodePtrA( nullptr );
    shared_ptr<Node> nodePtrB( nullptr );

    for ( const auto& nodeIt : nodes )
    {
        if ( nodePtrA == nullptr || nodePtrB == nullptr )
        {
            if ( nodeIt->id == nodeIdA )
            {
                if ( nodeIt->LinkedTo( nodeIdB ) )
                {
                    return cck::LinkError::NODES_ALREADY_LINKED;
                }
                else
                {
                    nodePtrA = nodeIt;
                }
            }
            else if ( nodeIt->id == nodeIdB )
            {
                if ( nodeIt->LinkedTo( nodeIdA ) )
                {
                    return cck::LinkError::NODES_ALREADY_LINKED;
                }
                else
                {
                    nodePtrB = nodeIt;
                }
            }
        }
        else
        {
            break;
        }
    }

    if ( nodePtrA == nullptr || nodePtrB == nullptr )
    {
        return cck::LinkError::ID_NOT_FOUND;
    }

    //Create temporary edge to test new Triangles
    shared_ptr<Edge> tempEdge( new Edge( nodePtrA, nodePtrB, mountainMinHeight, mountainMaxHeight, mountainRadius, mountainPlateau, globeRadius ) );
    tempEdge->AddSides();

    //Search for common neighbors of nodeA and nodeB that form a Triangle
    vector<shared_ptr<Node>> commonNeighbors = nodePtrA->FindCommonNeighbors( nodePtrB ); //TODO: Tidy this and below loop up

    for ( const auto& neighbor : commonNeighbors )
    {
        vector<shared_ptr<Edge>> commonEdges;
        //commonEdges.push_back( nodePtrA->GetLinkTo( nodePtrB->id )->edge );
        commonEdges.push_back( tempEdge );
        commonEdges.push_back( nodePtrB->GetLinkTo( neighbor->id )->edge );
        commonEdges.push_back( neighbor->GetLinkTo( nodePtrA->id )->edge );

        cck::Vec3 average = ( nodePtrA->position + nodePtrB->position + neighbor->position ) / 3.0;
        average = average.Unit();

        vector<shared_ptr<Side>> commonSides;

        //dot product with each edge side
        for ( const auto& edge : commonEdges )
        {
            vector<shared_ptr<Side>> edgeSides = edge->GetSides();
            for ( const auto& side : edgeSides )
            {
                if ( DotProduct( side->normal, average ) >= 0.0 )
                {
                    if ( side->FormsTriangle() )
                    {
                        return cck::LinkError::TRIANGLE_CONFLICT;
                    }
                    else
                    {
                        side->SetFormsTriangle();
                        commonSides.push_back( side );

                        if ( cck::DotProduct( side->edge->normal, average ) >= 0.0 )
                        {
                            side->edge->positiveMountain->SetInactive();
                        }
                        else
                        {
                            side->edge->negativeMountain->SetInactive();
                        }
                    }
                }
            }
        }
        triangles.push_back( std::make_shared<Triangle>( nodePtrA, nodePtrB, neighbor, commonSides, globeRadius ) );
    }

    nodePtrA->AddLink( std::make_shared<Link>( nodePtrB, tempEdge ) );
    nodePtrB->AddLink( std::make_shared<Link>( nodePtrA, tempEdge ) );
    edges.push_back( tempEdge );

    return cck::LinkError::SUCCESS;
}

void cck::Globe::SampleData( const double sampleLatitude, const double sampleLongitude, double& sampleHeight, int& sampleId ) const
{
    SampleData( cck::GeoCoord( sampleLatitude * cck::pi / 180.0, sampleLongitude * cck::pi / 180.0 ), sampleHeight, sampleId );
}

void cck::Globe::SampleData( const cck::GeoCoord& sampleCoord, double& sampleHeight, int& sampleId ) const
{
    const cck::Vec3 samplePoint = sampleCoord.ToCartesian( globeRadius );
    const double noiseValue = noise.ScaledOctaveNoise( samplePoint.x, samplePoint.y, samplePoint.z, noiseOctaves, noisePersistance, noiseFrequency, 0.0, 1.0 );

    for ( const auto& triangle : triangles )
    {
        if ( triangle->SampleData( sampleCoord, samplePoint, globeRadius, noiseValue, sampleHeight, sampleId ) )
        {
            return;
        }
    }

    double highestHeight = std::numeric_limits<double>::lowest();
    int highestId = -1;

    for ( const auto& edge : edges )
    {
        const double influence = edge->GetInfluence( sampleCoord, samplePoint, globeRadius );

        if ( influence > 0.0 )
        {
            double tempHeight = 0.0;
            int tempId = -1;
            edge->SampleData( sampleCoord, samplePoint, globeRadius, noiseValue * influence, tempHeight, tempId );
            if ( tempHeight >= highestHeight )
            {
                highestHeight = tempHeight;
                highestId = tempId;
            }
        }
    }

    for ( const auto& node : nodes )
    {
        const double influence = node->GetInfluence( sampleCoord, globeRadius );

        if ( influence > 0.0 )
        {
            double tempHeight = 0.0;
            int tempId = -1;
            node->SampleData( noiseValue * influence, tempHeight, tempId );
            if ( tempHeight >= highestHeight )
            {
                highestHeight = tempHeight;
                highestId = tempId;
            }
        }
    }

    sampleHeight = highestHeight;
    sampleId = highestId;
}

void cck::Globe::SampleInfluence( const double sampleLatitude, const double sampleLongitude, double& sampleInfluence ) const
{
    SampleInfluence( cck::GeoCoord( sampleLatitude * cck::pi / 180.0, sampleLongitude * cck::pi / 180.0 ), sampleInfluence );
}

void cck::Globe::SampleInfluence( const cck::GeoCoord& sampleCoord, double& sampleInfluence ) const
{
    sampleInfluence = 0.0;
    const cck::Vec3 samplePoint = sampleCoord.ToCartesian( globeRadius );

    for ( const auto& triangle : triangles )
    {
        double influence = triangle->GetInfluence( samplePoint );
        if ( influence > 0.0 )
        {
            sampleInfluence = 1.0;
            return;
        }
    }

    double greatestInfluence = 0.0;

    for ( const auto& edge : edges )
    {
        double influence = edge->GetInfluence( sampleCoord, samplePoint, globeRadius );
        if ( influence > greatestInfluence )
        {
            greatestInfluence = influence;
        }
    }

    for ( const auto& node : nodes )
    {
        double influence = node->GetInfluence( sampleCoord, globeRadius );
        if ( influence > greatestInfluence )
        {
            greatestInfluence = influence;
        }
    }

    sampleInfluence = greatestInfluence;
}

cck::NoiseError cck::Globe::SetNoiseParameters( const int octaves, const double persistance, const double frequency )
{
    if ( octaves <= 0 )
    {
        return cck::NoiseError::NON_POSITIVE_OCTAVES;
    }

    if ( persistance <= 0.0 )
    {
        return cck::NoiseError::NON_POSITIVE_PERSISTANCE;
    }

    if ( frequency <= 0.0 )
    {
        return cck::NoiseError::NON_POSITIVE_FREQUENCY;
    }

    noiseOctaves = octaves;
    noisePersistance = persistance;
    noiseFrequency = frequency;
    return cck::NoiseError::SUCCESS;
}

cck::Globe::Globe( const double globeRadius, const unsigned int seed )
    :   globeRadius( globeRadius ),
        noise( seed ),
        noiseOctaves( 7 ),
        noisePersistance( 0.6 ),
        noiseFrequency( 0.0001 )
{
}
