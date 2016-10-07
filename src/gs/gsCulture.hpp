#pragma once

#include "gsColor.hpp"
#include "gsVec3.hpp"

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
        static string GenerateCultureName(const int groupId);

    public:
        Culture(const int groupId);
    };
}
