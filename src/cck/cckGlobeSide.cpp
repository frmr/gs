#include "cckGlobe.h"
#include "cckMath.h"

bool cck::Globe::Side::FormsTriangle() const
{
    return formsTriangle;
}

void cck::Globe::Side::SetFormsTriangle()
{
    formsTriangle = true;
}

cck::Globe::Side::Side( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const shared_ptr<Edge>& edge )
    :   formsTriangle( false ),
        normal( cck::CrossProduct( nodeA->unitVec, nodeB->unitVec ).Unit() ),
        edge( edge )
{
}
