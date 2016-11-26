#pragma once

#include "gsMath.hpp"
#include <cassert>

namespace gs
{
	template<typename T>
	class Bounded
	{
	public:
		Bounded(const T& lower, const T& upper, const T& value, const bool clamp) :
			lower(lower),
			upper(upper),
			clamp(clamp),
			value(value)
		{
		}

		operator T() const
		{
			return value;
		}

		Bounded& operator=(const T& newValue)
		{
			value = newValue;

			if (clamp)
			{
				gs::Clamp(value, lower, upper);
			}

			assert(value >= lower && value <= upper);

			return *this;
		}

	protected:
		const T		lower;
		const T		upper;
		const bool	clamp;
		T			value;
	};
}