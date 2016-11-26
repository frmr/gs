#pragma once

#include "gsMath.hpp"
#include <cassert>

namespace gs
{
	template<typename T>
	class Unit
	{
	public:
		Unit(const T& value, const bool clamp) :
			clamp(clamp),
			value(value)
		{
			ClampAndValidate();
		}

		operator T() const
		{
			return value;
		}

		Unit<T> operator+(const T& rhs) const
		{
			return Unit<T>(value + rhs, clamp);
		}

		Unit<T> operator-(const T& rhs) const
		{
			return Unit<T>(value - rhs, clamp);
		}

		Unit<T> operator*(const T& rhs) const
		{
			return Unit<T>(value * rhs, clamp);
		}

		Unit<T> operator/(const T& rhs) const
		{
			return Unit<T>(value / rhs, clamp);
		}

		Unit<T> operator+(const Unit<T>& rhs) const
		{
			return Unit<T>(value + rhs, clamp);
		}

		Unit<T> operator-(const Unit<T>& rhs) const
		{
			return Unit<T>(value - rhs, clamp);
		}

		Unit<T> operator*(const Unit<T>& rhs) const
		{
			return Unit<T>(value * rhs, clamp);
		}

		Unit<T> operator/(const Unit<T>& rhs) const
		{
			return Unit<T>(value / rhs, clamp);
		}

		Unit<T>& operator+=(const T& rhs)
		{
			value += rhs;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator-=(const T& rhs)
		{
			value -= rhs;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator*=(const T& rhs)
		{
			value *= rhs;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator/=(const T& rhs)
		{
			value /= rhs;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator+=(const Unit<T>& rhs)
		{
			value += rhs.value;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator-=(const Unit<T>& rhs)
		{
			value -= rhs.value;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator*=(const Unit<T>& rhs)
		{
			value *= rhs.value;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator/=(const Unit<T>& rhs)
		{
			value /= rhs.value;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator=(const T& newValue)
		{
			value = newValue;
			ClampAndValidate();
			return *this;
		}

		Unit<T>& operator=(const Unit<T>& rhs)
		{
			value = rhs.value;
			ClampAndValidate();
			return *this;
		}

	protected:
		void ClampAndValidate()
		{
			if (clamp)
			{
				value = gs::Clamp(value, (T)0, (T)1);
			}

			assert(value >= (T)0 && value <= (T)1);
		}

	protected:
		const bool	clamp;
		T			value;
	};

	typedef Unit<float>		UnitF;
	typedef Unit<double>	UnitD;
}