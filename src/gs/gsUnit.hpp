#pragma once

#include "gsMath.hpp"
#include <cassert>

namespace gs
{
	template<typename T>
	class Unit
	{
	public:
		//Unit(const bool clamp) :
		//	clamp(clamp),
		//	value()
		//{
		//}

		Unit(const T& value, const bool clamp) :
			clamp(clamp),
			value(value)
		{
		}

		operator T() const
		{
			return value;
		}

		Unit& operator=(const T& newValue)
		{
			value = newValue;

			if (clamp)
			{
				gs::Clamp(value, (T)0, (T)1);
			}

			assert(value >= (T)0 && value <= (T)1);

			return *this;
		}

	private:
		const bool	clamp;
		T			value;
	};

	typedef Unit<float>		UnitF;
	typedef Unit<double>	UnitD;
}