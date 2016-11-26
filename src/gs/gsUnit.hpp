#pragma once

#include "gsBounded.hpp"

namespace gs
{
	template<typename T>
	class Unit : public gs::Bounded<T>
	{
	public:

		Unit(const T& value, const bool clamp) :
			gs::Bounded<T>((T)0, (T)1, value, clamp)
		{
		}

		Unit<T>& operator=(const T& newValue)
		{
			value = newValue;

			if (clamp)
			{
				gs::Clamp(value, lower, upper);
			}

			assert(value >= lower && value <= upper);

			return *this;
		}
	};

	typedef Unit<float>		UnitF;
	typedef Unit<double>	UnitD;
}