#ifndef GS_CULTURE_H
#define GS_CULTURE_H

#include "gsColor.h"
#include "gsVec3.h"

#include <string>

using std::string;

namespace gs
{
    class Culture
    {
    private:
        const int       groupId;
        const string    name;
        const gs::Color color;

    public:
        static string GenerateCultureName( const int groupId );

    public:
        Culture( const int groupId );
    };
}

#endif // GS_CULTURE_H
