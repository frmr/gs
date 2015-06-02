#include "cckGlobe.h"

bool cck::Globe::Link::LinksTo( const int nodeId ) const
{
    return ( target->id == nodeId ) ? true : false;
}

cck::Globe::Link::Link( const shared_ptr<Node>& target, const shared_ptr<Edge>& edge )
    :   target( target ),
        edge( edge )
{
}
