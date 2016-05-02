#include "gsTileGroup.h"

#include <iostream>

using std::cerr;
using std::endl;

GLuint gs::TileGroup::GetBufferBegin() const
{
    return bufferBegin;
}

GLuint gs::TileGroup::GetBufferEnd() const
{
    return bufferEnd;
}

gs::TileGroup::TileGroup(const GLuint bufferBegin)
    :   bufferBegin(bufferBegin),
        bufferEnd(0)
{
    cerr << "New tile group at " << bufferBegin << endl;
}
