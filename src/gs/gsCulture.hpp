#pragma once

#include "gsColor.hpp"
#include "gsRandomRange.hpp"
#include "gsVec3.hpp"

#include <string>

using std::string;

namespace gs
{
    class Culture
    {
    public:
		const uint16_t					id;
        const int						groupId;
        const string					name;
        const gs::Color					color;

	private:
		static gs::RandomRange<uint8_t>	randomRange;

    public:
        static string GenerateCultureName(const int groupId);

    public:
        Culture(const int id, const int groupId);
    };
}
