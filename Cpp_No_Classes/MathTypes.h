#pragma once

#include <cmath>
#include <type_traits>

namespace Math::Vector
{
	template<class T>
	struct _Vec2
	{
		static constexpr size_t num_elements = 2;
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		_Vec2() = default;
		_Vec2( T X, T Y )
			:
			x( X ), y( Y )
		{}

		T x = 0, y = 0;
	};

	template<class T>
	struct _Vec3
	{
		static constexpr size_t num_elements = 3;
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		_Vec3() = default;
		_Vec3( T X, T Y )
			:
			x( X ), y( Y )
		{}

		T x = 0, y = 0, z = 0;
	};
}

template<template<class> class vectype,class T>
struct num_elements
{
	static constexpr value = 1;
};
template<class T> struct num_elements<Math::Vector::_Vec2, T>
{
	static constexpr size_t value = 2;
};
template<class T> struct num_elements<Math::Vector::_Vec3, T>
{
	static constexpr size_t value = 3;
};

template<template<class> class vectype, class T>
struct is_vector
{
	static constexpr bool value = num_elements<vectype, T>::value > 1;
};

template<template<class> class vectype, class T>
struct cross_product_returns
{
	using type = vectype<T>;
};

template<class T>
struct cross_product_returns<Math::Vector::_Vec2, T>
{
	using type = T;
};

namespace Math::Rect
{
	template<class T>
	struct _Rect
	{
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		_Rect() = default;
		_Rect( T Left, T Top, T Right, T Bottom )
			:
			left( Left ), top( Top ), right( Right ), bottom( Bottom )
		{}

		T left = 0, top = 0, right = 0, bottom = 0;
	};

}
// TODO: Make physics stuffs
namespace
{
	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator+( const vectype<T>& lhs, const vectype<T>& rhs )
	{
		vectype<T> res;
		res.x = lhs.x + rhs.x;
		res.y = lhs.y + rhs.y;

		if constexpr( num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z + rhs.z;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator+=( vectype<T>& lhs, const vectype<T>& rhs )
	{
		lhs = lhs + rhs;

		return lhs;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator-( const vectype<T>& lhs, const vectype<T>& rhs )
	{
		vectype<T> res;
		res.x = lhs.x - rhs.x;
		res.y = lhs.y - rhs.y;

		if constexpr( num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z - rhs.z;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>&
		operator-=( vectype<T>& lhs, const vectype<T>& rhs )
	{
		lhs = lhs - rhs;
		return lhs;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator*( vectype<T>& lhs, const T rhs )
	{
		vectype<T> res;
		res.x = lhs.x * rhs;
		res.y = lhs.y * rhs;
		if constexpr( num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z * rhs;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>&
		operator*=( vectype<T>& lhs, const T rhs )
	{
		lhs = lhs * rhs;
		return lhs;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator/( vectype<T>& lhs, const T rhs )
	{
		if constexpr( std::is_floating_point_v<T> )
		{
			return lhs * ( static_cast< T >( 1 ) / rhs );
		}

		vectype<T> res;
		res.x = lhs.x / rhs;
		res.y = lhs.y / rhs;

		if constexpr( num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z / rhs;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>&
		operator/=( vectype<T>& lhs, const T rhs )
	{
		lhs = lhs / rhs;

		return lhs;
	}
}

namespace Math
{
	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector<vectype,T>::value, T>
		DotProduct( const vectype<T>& lhs, const vectype<T>& rhs )
	{
		using vec = vectype<T>;
		static_assert( is_vector<vec>::value, "DotProduct must have a math vector type." );

		if constexpr( vec::num_elements == 2 )
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		else if constexpr ( vec::num_elements == 3 )
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		else
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector<vectype, T>::value, T>
		LengthSq( const vectype<T>& lhs )
	{
		return DotProduct( lhs, lhs );
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector<vectype, T>::value, T>
		Length( const vectype<T>& lhs )
	{
		return std::sqrt<T>( DotProduct( lhs, lhs ) );
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector<vectype, T>::value, T>
		DistanceSq( const vectype<T>& StartPt, const vectype<T>& EndPt )
	{
		return LengthSq( EndPt - StartPt );
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector<vectype, T>::value, T>
		DistanceSq( const vectype<T>& StartPt, const vectype<T>& EndPt )
	{
		return Length( EndPt - StartPt );
	}

	template<template<class> class vectype, class T>
	auto CrossProduct( const vectype<T>& lhs, const vectype<T>& rhs )->
		typename cross_product_returns<vectype, T>::type
	{
		using vec = vectype<T>;
		if constexpr( vec::num_elements == 2 )
		{
			return lhs.y * rhs.x - lhs.x - rhs.y;
		}

		return vec{
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		};
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector<vectype,T>::value,vectype<T>>
		Normalize( const vectype<T>&lhs )
	{
		const auto len = Length( lhs );

		if( len == static_cast< T >( 0 ) )
			return lhs;

		return lhs * ( static_cast< T >( 1 ) / len );
	}

	template<class T>
	bool Overlaps( const Rect::_Rect<T>& r1, const Rect::_Rect<T>& r2 )
	{
		return
			r1.left < r2.right && r1.right > r2.left &&
			r1.top < r2.bottom && r1.bottom > r2.top;
	}

	template<class T>
	bool Contians( const Rect::_Rect<T>& r1, const Rect::_Rect<T>& r2 )
	{
		const auto res_v2 =
			r1.left < r2.left && r1.right > r2.right &&
			r1.top < r2.top && r1.bottom > r2.bottom;
	}

	template<class T>
	bool Contians( const Rect::_Rect<T>& rect, const Math::Vector::_Vec2<T>& pt )
	{
		const auto res_v2 =
			rect.left < pt.x && rect.right > pt.x &&
			rect.top < pt.y && rect.bottom > pt.y;
	}

}