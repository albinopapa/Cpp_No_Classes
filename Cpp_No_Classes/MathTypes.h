#pragma once

#include <cmath>
#include <type_traits>
#include <algorithm>

namespace Math::Vector
{
	template<class T>
	struct _Vec2
	{
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		constexpr _Vec2() = default;
		constexpr _Vec2( T X, T Y )
			:
			x( X ), y( Y )
		{}

		T x = 0, y = 0;
	};

	template<class T>
	struct _Vec3
	{
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		constexpr _Vec3() = default;
		constexpr _Vec3( T X, T Y )
			:
			x( X ), y( Y )
		{}

		T x = 0, y = 0, z = 0;
	};
	
	template<template<class> class vectype, class T> struct num_elements
	{
		static constexpr size_t value = 1;
	};
	template<class T> struct num_elements<_Vec2, T>
	{
		static constexpr size_t value = 2;
	};
	template<class T> struct num_elements<_Vec3, T>
	{
		static constexpr size_t value = 3;
	};

	template<template<class> class vectype, class T> struct is_vector
	{
		static constexpr bool value = num_elements<vectype, T>::value > 1;
	};

	template<template<class> class vectype, class T> struct cross_product_returns
	{
		using type = vectype<T>;
	};

	template<class T> struct cross_product_returns<_Vec2, T>
	{
		using type = T;
	};

	template<template<class> class vectype, class scalartype>
	constexpr bool is_vector_v = is_vector<vectype, scalartype>::value;

	template<template<class> class vectype, class T>
	using cross_product_returns_t = typename cross_product_returns<vectype, T>::type;

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<is_vector_v<vectype, T>, T>
		DotProduct( const vectype<T>& lhs, const vectype<T>& rhs )
	{
		static_assert( is_vector_v<vectype,T>, "DotProduct must have a math vector type." );

		if constexpr( num_elements<vectype,T>::value == 2 )
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		else if constexpr ( num_elements<vectype, T>::value == 3 )
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		else
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<is_vector_v<vectype, T>, T>
		LengthSq( const vectype<T>& lhs )
	{
		return DotProduct( lhs, lhs );
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector_v<vectype, T>, T>
		Length( const vectype<T>& lhs )
	{
		auto dp_res = DotProduct( lhs, lhs );
		return std::sqrt( dp_res );
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<is_vector_v<vectype, T>, T>
		DistanceSq( const vectype<T>& StartPt, const vectype<T>& EndPt )
	{
		return LengthSq( EndPt - StartPt );
	}

	template<template<class> class vectype, class T>
	std::enable_if_t<is_vector_v<vectype, T>, T>
		Distance( const vectype<T>& StartPt, const vectype<T>& EndPt )
	{
		return Length( EndPt - StartPt );
	}

	template<template<class> class vectype, class T>
	constexpr auto CrossProduct( 
		const vectype<T>& lhs, const vectype<T>& rhs )->cross_product_returns_t<vectype, T>
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
	std::enable_if_t<is_vector_v<vectype, T>, vectype<T>>
		Normalize( const vectype<T>&lhs )
	{
		const auto len = Length( lhs );

		if( len == static_cast< T >( 0 ) )
			return lhs;

		return lhs * ( static_cast< T >( 1 ) / len );
	}

}


namespace Math::Rect
{
	template<class T>
	struct _Rect
	{
		static_assert( std::is_arithmetic_v<T>, "T must be arithmatic type." );

		constexpr _Rect() = default;
		constexpr _Rect( T Left, T Top, T Right, T Bottom )
			:
			left( Left ), top( Top ), right( Right ), bottom( Bottom )
		{}

		T left = 0, top = 0, right = 0, bottom = 0;
	};

	template<class T>
	constexpr bool Overlaps( const _Rect<T>& r1, const _Rect<T>& r2 )
	{
		return
			r1.left < r2.right && r1.right > r2.left &&
			r1.top < r2.bottom && r1.bottom > r2.top;
	}

	template<class T>
	constexpr bool Contians( const _Rect<T>& r1, const _Rect<T>& r2 )
	{
		const auto res_v2 =
			r1.left < r2.left && r1.right > r2.right &&
			r1.top < r2.top && r1.bottom > r2.bottom;
	}

	template<class T>
	constexpr bool Contians( const _Rect<T>& rect, const Vector::_Vec2<T>& pt )
	{
		const auto res_v2 =
			rect.left < pt.x && rect.right > pt.x &&
			rect.top < pt.y && rect.bottom > pt.y;
	}

	template<class T>
	_Rect<T> Translate( const _Rect<T>& rect, const Vector::_Vec2<T>& delta )
	{
		return { 
			rect.left + delta.x,
			rect.top + delta.y,
			rect.right + delta.x,
			rect.bottom + delta.y 
		};
	}
}

// TODO: Make physics stuffs
namespace
{
	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator+( const vectype<T>& lhs, const vectype<T>& rhs )
	{
		vectype<T> res;
		res.x = lhs.x + rhs.x;
		res.y = lhs.y + rhs.y;

		if constexpr( Math::Vector::num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z + rhs.z;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T> &&
		Math::Vector::is_vector_v<vectype,T>, vectype<T>>
		operator+=( vectype<T>& lhs, const vectype<T>& rhs )
	{
		lhs = lhs + rhs;

		return lhs;
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator-( const vectype<T>& lhs, const vectype<T>& rhs )
	{
		vectype<T> res;
		res.x = lhs.x - rhs.x;
		res.y = lhs.y - rhs.y;

		if constexpr( Math::Vector::num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z - rhs.z;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>&
		operator-=( vectype<T>& lhs, const vectype<T>& rhs )
	{
		lhs = lhs - rhs;
		return lhs;
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator*( const vectype<T>& lhs, const T rhs )
	{
		vectype<T> res;
		res.x = lhs.x * rhs;
		res.y = lhs.y * rhs;
		if constexpr( Math::Vector::num_elements<vectype, T>::value == 3 )
		{
			res.z = lhs.z * rhs;
		}

		return res;
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>&
		operator*=( vectype<T>& lhs, const T rhs )
	{
		lhs = lhs * rhs;
		return lhs;
	}

	template<template<class> class vectype, class T>
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>
		operator/( const vectype<T>& lhs, const T rhs )
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
	constexpr std::enable_if_t<std::is_arithmetic_v<T>, vectype<T>>&
		operator/=( vectype<T>& lhs, const T rhs )
	{
		lhs = lhs / rhs;

		return lhs;
	}
}

namespace Math
{

	template<class T>
	T clamp( const T value, const T min_value, const T max_value )
	{
		return std::max( min_value, std::min( value, max_value ) );
	}
}

namespace Math
{
	template<int32_t Den>
	struct Percent
	{
		static constexpr int32_t denominator = Den;
		int32_t numerator = 0;
	};

	template<class T>
	constexpr int32_t get( const T perc )
	{
		return ( perc.numerator * 100 ) / perc.denominator;
	}

}

using Vec2f = Math::Vector::_Vec2<float>;
using Vec2i = Math::Vector::_Vec2<int32_t>;
using Point = Math::Vector::_Vec2<int32_t>;

using Rectf = Math::Rect::_Rect<float>;
using Recti = Math::Rect::_Rect<int32_t>;
